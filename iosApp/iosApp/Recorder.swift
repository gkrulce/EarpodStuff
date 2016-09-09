//
//  Recorder.swift
//  iosApp
//
//  Created by Galen Krulce on 9/8/16.
//  Copyright © 2016 Galen Krulce. All rights reserved.
//

import Foundation
import AVFoundation

class Recorder : NSObject, AVAudioRecorderDelegate {
    
    enum State {
        case INIT
        case STARTING
        case RECORDING
        case FINISHED
        case FAILURE
    };
    
    struct Event {
        let event: String
        let time: NSDate
    };
    
    
    var state: State = State.INIT
    var recordingSession: AVAudioSession!
    var audioRecorder: AVAudioRecorder!
    var startTime: NSDate!
    var endTime: NSDate!
    var prevVolume: Float = 0
    var events = [Event]()
    let statusCb: (status: String)->Void
    
    init(statusCb: (status: String)->Void) {
        self.statusCb = statusCb
    }
    
    func startRecording() {
        if(state != State.INIT) {
            statusCb(status: "Called startRecording when already started")
            return
        }
        state = State.STARTING;
        initMic({[unowned self](success: Bool) -> Void in
            if(!success) {
                print("Failed to init microphone; bailing");
                self.statusCb(status: "Failed to initialize microphone")
                self.state = State.FAILURE;
                return;
            }
            print ("Starting to record!")
            self.statusCb (status: "Starting to record")
            let audioURL = self.getDocumentsDirectory().URLByAppendingPathComponent("recording.pcm")
            let settings = [
                AVFormatIDKey: Int(kAudioFormatLinearPCM),
                AVSampleRateKey: 44100,
                AVNumberOfChannelsKey: 1 as NSNumber,
                AVLinearPCMBitDepthKey: 16,
                AVLinearPCMIsBigEndianKey: true,
                AVLinearPCMIsFloatKey: false,
            ]
            
            do {
                self.audioRecorder = try AVAudioRecorder(URL: audioURL, settings: settings)
                self.audioRecorder.delegate = self
                self.audioRecorder.record()
                self.startTime = NSDate()
                self.state = State.RECORDING
            } catch {
                print("Error encountered when recording")
                self.finish(success: false)
            }
            
            });
    }
    
    func finishRecording() {
        if(state != State.RECORDING) {
            statusCb(status: "Called finishRecording when not recording!")
            return
        }
        finish(success: true)
    }
    
    private func initMic(cb: (success: Bool)->Void) {
        recordingSession = AVAudioSession.sharedInstance()
        
        do {
            try recordingSession.setCategory(AVAudioSessionCategoryPlayAndRecord)
            try recordingSession.setActive(true)
            prevVolume = recordingSession.outputVolume
            recordingSession.addObserver(self, forKeyPath: "outputVolume",
                                         options: NSKeyValueObservingOptions.New, context: nil)
            recordingSession.requestRecordPermission() { (allowed: Bool) -> Void in
                dispatch_async(dispatch_get_main_queue()) {
                    if allowed {
                        print("Succeeded in requesting microphone permissions!")
                        cb(success: true);
                    } else {
                        print("Failed to request microphone permissions!")
                        cb(success: false);
                    }
                }
            }
        } catch {
            print("Exception raised while requesting microphone permission!")
            cb(success: false);
        }
    }
    
    private func finish(success success: Bool) {
        audioRecorder.stop()
        audioRecorder = nil
        endTime = NSDate()
        statusCb(status: "Done recording")
        if(!success) {
            state = State.FAILURE;
        }else {
            state = State.FINISHED;
        }
        serializeEvents(events)
        
    }
    
    private func serializeEvents(events: [Event]) {
        print ("Number of events captured:  \(events.count)")
        var eventString : String = ""
        var metadataString: String = ""
        // Write events to string
        for event in events {
            eventString += "\(event.event)\t\(event.time.timeIntervalSinceDate(startTime))\n"
        }
        
        // Write metadata to string
        metadataString += "\(startTime.description)\tDate of capture\n"
        metadataString += "\(events.count)\tEvents captured\n"
        metadataString += "\(endTime.timeIntervalSinceDate(startTime))\tRecording time\n"
        metadataString += "\(endTime.timeIntervalSinceDate(startTime)*44100*2)\tApprox file size from recording time\n"
        
        do {
            let eventsFile = self.getDocumentsDirectory().URLByAppendingPathComponent("events.csv")
            try eventString.writeToURL(eventsFile, atomically: false, encoding: NSUTF8StringEncoding)
            let metadataFile = self.getDocumentsDirectory().URLByAppendingPathComponent("metadata.txt")
            try metadataString.writeToURL(metadataFile, atomically: false, encoding: NSUTF8StringEncoding)
        }
        catch {
            print("Error when writing files!")
            return
        }
    }
    
    override func observeValueForKeyPath(keyPath: String?, ofObject object: AnyObject?, change: [String : AnyObject]?, context: UnsafeMutablePointer<Void>) {
        let currTime = NSDate()
        if keyPath == "outputVolume"{
            if recordingSession.outputVolume == prevVolume {
                print ("Strange, volume stayed the same?")
                return
            }
            var event: Event
            if recordingSession.outputVolume < prevVolume {
                print ("Volume went down!")
                statusCb(status: "Saw volume up!")
                event = Event(event:"VolDown", time:currTime)
            }else {
                print ("Volume went up!")
                statusCb(status: "Saw volume down!")
                event = Event(event:"VolUp", time:currTime)
            }
            events.append(event)
            
        }
    }
    
    func audioRecorderDidFinishRecording(recorder: AVAudioRecorder, successfully flag: Bool) {
        if (!flag) {
            finish(success: false)
        }
    }
    
    // TODO generate a random directory
    private func getDocumentsDirectory() -> NSURL {
        let paths = NSSearchPathForDirectoriesInDomains(.DocumentDirectory, .UserDomainMask, true)
        let documentsDirectory = paths[0]
        return NSURL(fileURLWithPath: documentsDirectory)
    }
}