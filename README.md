# WATER-LEVEL-MANAGEMENT-SYSTEM
Water Level Management System with Arduino Uno and GSM module
## Table of content
  * [Overview](#overview)
  * [Problem Statement](#problem_statement)
  * [Procedure](#procedure)
  * [Design Layout](#design_layout)
  * [Test Results](#test_results)
  * [Limitation](#limitation)

## Overview
This is Water Level Management System with Arduino Uno and GSM module to determine the level of water in an overhead water tank.

## Problem Statement
Most hostels in and out of university campuses often get water shortage crises due to the fact that the managers do not have the necessary tools to check the water level at specific time intervals and also do not know the average consumption per week. This has proven to be quiet the problem which causes student to get their regular routines/ daily activities interrupted. Therefore, there is need to help the managers manage the water level automatically rather than the manual way.
The overhead tank is made up opaque material to prevent algae growth and is closed with lid to protect from dust and infestation from other organism. So, the water level in the tank is unknown. Ultrasonic sensor provides non-contact water level measurement.

## Procedure
  * Install ArduIno IDE on the desktop.
  * Install softwareserial
  * Fix the basic components you need according to the schematics/Layout provided.
  * Adjust the code according to the dimensions to container/tank you are using.
  * Upload the code to the the arduino uno board.
  * Test the results using your mobile phone

## Design Layout

## Test Results
   Procedure                                    ExpectedResults	                                                Validation
  * User request data using “STATE”	--SMS indicating the percentage used, percentage left and average readings--Valid
  * Water in the overhead tank is equal to or greater than 100%	 --SMS “The Tank is full.”sent and RGB LED indicates green.--	Valid
  * Water in the overhead tank is inbetween 20% and 99%	--SMS “There is water in the tank.”sent and RGB LED indicates blue--	Valid
  * Water in the overhead tank is inbetween 5 % and 20%	--SMS “The Tank is critically low.”sent and RGB LED indicates red for 10 seconds--	Valid
  * Water in the overhead tank is below 5 %	--SMS “The Tank is empty.”sent and RGB LED indicates red for 10 seconds repeatedly.--	Valid

  Test results Images
  <!---Add image--->
  * ![Study for Finals]
  * ![Study for Finals]
  * ![Study for Finals]
  * ![Study for Finals]

## Limitation
  * The ultrasonic sensor (HC-SR04) used in this work measures distance in the range of 2cm to 400cm with accuracy of 3mm.
  * The GSM module (SIM900D) uses prepaid SIM card which gets depleted over time so needs the user to buy airtime.
  * It also needs high power supply.
