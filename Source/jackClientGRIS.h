/*
 This file is part of ServerGris.
 
 Developers: Olivier Belanger, Nicolas Masson
 
 ServerGris is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 ServerGris is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with ServerGris.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef jackClientGris_h
#define jackClientGris_h

#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __linux__
#include <mutex>
#endif

#include "../JuceLibraryCode/JuceHeader.h"

#include <jack/jack.h>
#include <jack/transport.h>
#include <jack/types.h>
#include <jack/session.h>

#include "vbap.h"

class Speaker;
using namespace std;

//Limit SpatServer In/Out
static unsigned int const MaxInputs  = 256;
static unsigned int const MaxOutputs = 256;

struct Client {
    String          name;
    unsigned int    portStart     = 0;
    unsigned int    portEnd       = 0;
    unsigned int    portAvailable = 0;
    bool            initialized   = false;
    bool            connected     = false;
};

struct SourceIn {
    unsigned int id;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    float azimuth = 0.0f;
    float zenith = 0.0f;
    float radius = 0.0f;
    
    float aziSpan = 0.0f;
    float zenSpan = 0.0f;
    
    bool  isMuted = false;
    bool  isSolo = false;
    float gain; //Not Implemented

    int directOut = 0;
    
    VBAP_DATA * paramVBap;
};

struct SpeakerOut {
    unsigned int id;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    float azimuth = 0.0f;
    float zenith = 0.0f;
    float radius = 0.0f;

    float gain = 1.0f;

    bool hpActive = false;
    double b1 = 0.0;
    double b2 = 0.0;
    double b3 = 0.0;
    double b4 = 0.0;
    double ha0 = 0.0;
    double ha1 = 0.0;
    double ha2 = 0.0;

    bool  isMuted = false;
    bool  isSolo = false;

    int outputPatch = 0;

    bool directOut = false;
};

//Mode Spat
typedef enum {
    VBap = 0,
    DBap,
    VBap_HRTF,
    STEREO
} ModeSpatEnum;

//Settings Jack Server
static const StringArray BufferSize = {"32", "64", "128", "256", "512", "1024", "2048"};
static const StringArray RateValues = {"44100", "48000", "88200", "96000"};
static const StringArray FileFormats = {"WAV", "AIFF"};

class AudioRecorder
{
public:
    AudioRecorder() : backgroundThread ("Audio Recorder Thread"), activeWriter (nullptr)
    {
        backgroundThread.startThread();
    }

    ~AudioRecorder()
    {
        stop();
    }

    //==============================================================================
    void startRecording (const File& file, unsigned int sampleRate, String extF)
    {
        stop();

        // Create an OutputStream to write to our destination file...
        file.deleteFile();
        ScopedPointer<FileOutputStream> fileStream(file.createOutputStream());

        AudioFormatWriter *writer;

        if (fileStream != nullptr)
        {
            // Now create a writer object that writes to our output stream...
            if (extF == ".wav") {
                WavAudioFormat wavFormat;
                writer = wavFormat.createWriterFor(fileStream, sampleRate, 1, 24, NULL, 0);
            } else {
                AiffAudioFormat aiffFormat;
                writer = aiffFormat.createWriterFor(fileStream, sampleRate, 1, 24, NULL, 0);
            }

            if (writer != nullptr)
            {
                fileStream.release(); // (passes responsibility for deleting the stream to the writer object that is now using it)

                // Now we'll create one of these helper objects which will act as a FIFO buffer, and will
                // write the data to disk on our background thread.
                threadedWriter = new AudioFormatWriter::ThreadedWriter (writer, backgroundThread, 32768);

                // And now, swap over our active writer pointer so that the audio callback will start using it..
                const ScopedLock sl (writerLock);
                activeWriter = threadedWriter;
            }
        }
    }

    void stop()
    {
        if (activeWriter == nullptr) { return; }

        // First, clear this pointer to stop the audio callback from using our writer object..
        {
            const ScopedLock sl (writerLock);
            activeWriter = nullptr;
        }

        // Now we can delete the writer object. It's done in this order because the deletion could
        // take a little time while remaining data gets flushed to disk, so it's best to avoid blocking
        // the audio callback while this happens.
        threadedWriter = nullptr;
    }

    void recordSamples(float **samples, int numSamples)
    {
        const ScopedLock sl (writerLock);
        activeWriter->write (samples, numSamples);
    }

private:
    TimeSliceThread backgroundThread; // the thread that will write our audio data to disk
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter; // the FIFO used to buffer the incoming data
    CriticalSection writerLock;
    AudioFormatWriter::ThreadedWriter* volatile activeWriter;
};


class jackClientGris {
public:

    //Jack var
    jack_client_t *client;
    
    vector<jack_port_t *> inputsPort;
    vector<jack_port_t *> outputsPort;
    
    float interMaster;

    //Pink Noise Sound
    float c0;
    float c1;
    float c2;
    float c3;
    float c4;
    float c5;
    float c6;
    float pinkNoiseGain;

    // Highpass filter
    double x1[MaxOutputs];
    double x2[MaxOutputs];
    double x3[MaxOutputs];
    double x4[MaxOutputs];
    double y1[MaxOutputs];
    double y2[MaxOutputs];
    double y3[MaxOutputs];
    double y4[MaxOutputs];

    //Mute Solo Vu meter
    float levelsIn[MaxInputs];
    float levelsOut[MaxOutputs];
    
    bool soloIn;
    bool soloOut;
    
    float masterGainOut;
    //------------------------
    
    vector<Client> listClient;
    mutex          lockListClient;
    SourceIn   listSourceIn   [MaxInputs];
    SpeakerOut listSpeakerOut [MaxOutputs];
    
    bool         processBlockOn;
    ModeSpatEnum modeSelected;
    
    bool noiseSound;
    bool autoConnection;
    bool overload;
    
    unsigned int sampleRate;
    unsigned int bufferSize;
    unsigned int numberInputs;
    unsigned int numberOutputs;
    unsigned int maxOutputPatch;
    vector<int> outputPatches;

    vector<vector<int>> vbap_triplets;

    //-------- VBAP+HRTF data ------------------------
    unsigned int hrtf_count[16];
    float hrtf_input_tmp[16][128];
    float vbap_hrtf_left_impulses[16][128];
    float vbap_hrtf_right_impulses[16][128];

    //-------- STEREO data ------------------------
    float last_azi[MaxInputs];

    //---------------------------------
    jackClientGris(unsigned int bufferS = 1024);

    virtual ~jackClientGris();
    
    bool  isReady() { return clientReady; }
    float getCpuUsed() const { return jack_cpu_load(client); }
    float getLevelsIn(int index) const { return levelsIn[index]; }
    float getLevelsOut(int index) const { return levelsOut[index]; }
    
    void resetHRTF();

    void addRemoveInput(unsigned int number);
    void clearOutput();
    bool addOutput(unsigned int outputPatch);
    void removeOutput(int number);

    void disconnectAllClient();
    void autoConnectClient();
    void connectionClient(String name, bool connect = true);
    void updateClientPortAvailable(bool fromJack);
    
    string getClientName(const char * port);
    unsigned int getPortStartClient(String nameClient);
    
    //Recording param =========================
    void prepareToRecord();
    void startRecord() { this->indexRecord = 0; this->recording = true; }
    void stopRecord() { this->recording = false; };

    AudioRecorder recorder[MaxOutputs];
    unsigned int indexRecord = 0;
    bool recording;
    void setRecordFormat(int format) { this->recordFormat = format; };
    int getRecordFormat() { return this->recordFormat; };
    void setRecordingPath(String filePath) { this->recordPath = filePath; }
    String getRecordingPath() { return this->recordPath; }
    bool isSavingRun() { return this->recording; };

    //SpeakerLoad
    unsigned int vbapDimensions;
    bool initSpeakersTripplet(vector<Speaker *>  listSpk, int dimensions, bool needToComputeVbap);
    void updateSourceVbap(int idS);
    int vbapSourcesToUpdate[MaxInputs];
    
private:
    
    bool clientReady;
    int recordFormat = 0; // 0 = WAV, 1 = AIFF
    String recordPath = "";
    VBAP_DATA * paramVBap;

    void connectedGristoSystem();
    
};

#endif
