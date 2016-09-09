//
//  ViewController.swift
//  iosApp
//
//  Created by Galen Krulce on 8/27/16.
//  Copyright © 2016 Galen Krulce. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    let recorder: Recorder = Recorder(statusCb:{(status: String) in
        print("statusCb: \(status)")
    })

    @IBAction func StartButtonPressed(sender: AnyObject) {
        recorder.startRecording()
            }
    @IBAction func StopButtonPressed(sender: AnyObject) {
        recorder.stopRecording()
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    


}

