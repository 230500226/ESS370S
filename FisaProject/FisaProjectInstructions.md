# Question 1  
**100 Points**  
The design and evaluation of a solar powered washing machine

---

## Relevant background information

A solar system consist of solar panels, batteries and an “inverter” that controls and interconnect the other subsystems. Solar power is becoming more popular for a variety of reasons. Solar panels give electricity according to the amount of sunlight falling on it. If the sun does not shine then the solar panels does not provide electrical energy. The power provided, by the solar panels, then has to be shared among the appliances. Thus, appliances working on solar energy will have to share the available energy depending on the household requirements and the availability of sunlight. If the sun does not shine then the solar panels does not provide electrical energy. If more than enough electrical power can be produced by the solar panels and the batteries is not fully charged then the extra energy is used to charge the batteries. If more than enough energy can be produced by the solar panels and the batteries is fully charged then the extra energy goes to waste. If the solar panels is not providing enough energy then the batteries has to add the rest. For example if the solar panels is delivering 1.8 kW and the household requires 2.2 kW then the batteries has to provide the 0.4 kW that is still required.

As you can see with your cell phone, the more you discharge a battery the shorter the lifespan is of the battery. Batteries are also expensive. Considering the price per kwh for batteries and municipality, in some cases, using the batteries can be more expensive than using municipal power. If the batteries are flat and the solar panels is not providing electricity, then the solar systems switches to the municipal power.

---

## Instructions to students

You are required to build a “washing machine”. This washing machine must have the ability to save the customer money. Your project has no control over the solar system except for the how much power your washing machine is withdrawing. The solar system indicate to the washing machine how much unused electrical energy is available. This is the only information transferred between the solar system and the washing machine. For example, if the batteries are charging because they are flat then the solar system has to provide a zero to the Arduino. The values transferred is from zero to three. An increase in value is synonymous with an increase in available electrical power. Assume the inverter sends out an infrared signal to the Arduino. The infrared sensor is connected to pin three of the Arduino.

A crucial part of your responsibilities is designing the controller for the washing machine. You also have to decide on which components to use to emulate the other components of the washing machine. For example, you have to decide which component, that is available on Tinkercad, will be able to emulate the direct drive of the washing machine. Except for doing its normal functions, this controller has to minimise using more energy than what is available from the solar system. Therefore, you have to determine how to minimise using energy when the solar panels does not provide all the required energy for the whole household. You also have to consider what the washing machine must do when the solar panels change from been unable to provide enough energy to be able to provide enough energy. Marks will be deducted if you import a stepper motor library. Assume the washing machine is always washing using cold water.

---

## Physical Hardware

Physical components what is commonly available to connect to an Arduino, has to be used to emulate the components of the washing machine. Thus, you have to use the most similar components.  

- As mentioned, you have to determine which component on Tinkercad is the most similar to the direct drive motor of the washing machine.  The connection of power to the Arduino and other devices should be correct.  The pins on the Arduino that controls the direct drive should be pins 4,5,6 and 7.
- You have to decide which device found on Tinkercad is the best for emulating the water pump in a washing machine.  Use pin 9 on the Arduino to control the water pump.
- An LED can show when the valve for the water is open.  Thus, an LED will emulate the valve.  Connect the valve indicator to pin eight.
- The start button of the washing machine is a push button.  The start/pause button should be connected to pin 10 of the Arduino
- Use a potentiometer as the sensor that detects the water level in the washing machine.  Connect the water level sensor to pin A5 of the Arduino.
- Your infrared communications has to be connected to pin number 3.

For the physical device, you will need to book out the components from the senior technician (Mr. Wills). After constructing the circuit and taking a photo, take the components back to the senior technician.

---

## Simulation of Hardware

Use components found in Tinkercad to emulate the components of the washing machine. Thus, you have to use the most similar components. The only difference between the simulation of the circuit and the physical circuit should be the direct drive. For the direct drive of the washing machine’ simulation, you have to use four LEDs. The rest of the components is the same as for the physical hardware.

---

## Software

After the user have inserted the soap and the clothes into the drum of the washing machine, the user presses the start button. That is all the user has to do until the washing machine is finished washing. The software should let the washing machine go through all the necessary cycles. The washing machine has the following sequential cycles: washing, rinsing and spinning. To save on time, each procedure should take less than 1 minute. A single cycle can include multiple sequential procedures. You have to write a function for each of these procedures. The procedures are:

- Filling drum with water: This is the first thing the washing machine should do after the start button is pressed.  The name of the function is fillDrum().  You have to determine which components to use and in which way.
- Washing: The name of the function that must be used for this cycle must be wash_rinse().  During this time, the drum must turn at a moderate pace.  It must first turn clockwise for three full turns.  Then it must turn anticlockwise for three full turns. Repeat these processes three times.  To achieve these rotations will be different for the simulation compared to the physical as some motors contain gearboxes.  Thus, to have an output of three whole rotations the motor most likely have to turn many more times.  The program you have to provide should cater for the simulation and not the physical.  Make the steps slow enough for a human to follow.  Each time before the drum changes direction, the drum should stop for one second.  Do not use the stepper motor’s library.
- Draining water from drum:  After washing, drain all the dirty water from the drum.  You have to determine which components to use for this function.  The name of the function is draining().
- Filling drum with water:  Fill the drum with clean water.  The name of the function must be “fillDrum()”.
- Rinsing: The goal with this cycle is to get all the soap out of the clothes.  The only difference between this cycle and the washing cycle is that the user does not put soap into the washing machine at this stage.  Thus, functionally this process is identical to the washing cycle.  What is show on the serial output should be different.
- Draining water from the drum: This is identical to what happened previously.
- Spinning: The goal is to get as much water as possible out of the clothes.  During this cycle the drum is spinning at maximum speed in one direction.  The drum should rotate twice as fast as for the other phases.  You are required to let the drum rotate 20 full revolutions.  You also have to consider what else is required during this phase.  Afterwards the washing machine must say, “finished”.  After finishing, the washing machine should be able to restart.
- Infrared Communications.  Create a function called checkPower(int requiredPowerLevel, int pinNum).  This function should pass to it the amount of power that is required by the washing machine to perform the specific cycle.  This function should then get another power value from the infrared devices.   As will be indicated, these power levels is indicated with values from zero to three.  In some washing cycles, the number of the affected pin might be required.
- Serial communication.  The serial display should show with which cycle the washing machine is busy.  An example of a printout can be “Filling drum before rinsing”.  These printouts are incorporated to ease the processes of debugging and marking.

It is recommended that the controller should react on water levels of above 80% and below 10%.   The washing machine should not continue to execute a procedure if the available electrical power is insufficient for that specific procedure. Different procedures can require different amounts of power. You will have to place the power ratings of each procedure in one of four categories. Category 0 is when the batteries are charging. Category 1 is synonymous with a very small amount of electricity. Category 3 is for the most electricity the washing machine will use. Category 2 is when the washing machine is using more electricity than for category 1 but less than for category 3. The power rating indicated by the solar system can then be from zero to three. Thus, the solar system should provide a number between zero and three to the washing machine. The power ratings of the solar system then corresponds to the power rating of the washing machine.  

---

## Simulation

For the drum’s motor in the Tinkercad simulation, use four LEDs that has opposing polarities. Use green LEDs for the simulation of the drum’s motor. Connect the “drum’s motor” to pins 4 to 7. Make sure the code that is used for the simulation’s drum’s motor can also be used for the physical drum’s motor. 

Use a potentiometer to emulate the sensor that detects the amount of water in the drum. Connect pin A5 to the potentiometer. Use a blue LED for the inlet valve. This valve has to let the drum fill with water until it is 80% full of water. Connect this LED to pin eight. Connect the pump to pin number 9 of the Arduino. A start button connected to pin 10. 

The student has to act as if he/she is the solar-system by “manually” providing information on available electrical energy to the washing machine. 

---

## Responsibility

This is the second of two documents that you have to submit. The first document form is the Graduate Attribute record. The Graduate Attribute document is the record that needs to show your investigation into the project before you started while this document is to show what you have accomplished.  The FISA mark, thus the mark for the project without the GA, is T4 on MAS or OPA. 

- Picture of Simulated project:  Setup a circuit using Tinkercad.  It refers to the simulation of the physical circuit.  In this way the program can be tested before the physical device is build and programed.  Add a picture of the simulated circuit to the FISA record.  The picture should be of such a nature that all components and connections can be easily identifiable.  This picture of the simulation will contribute 10% to the FISA mark as indicated by the table below.  This table functions as the rubric to the FISA project.
- Picture of Physical device:  Use an Arduino and other devices to create components that forms part of the “washing machine”.  Add a picture of the physical circuit to the FISA record.  The picture should be of such a nature that all components and connections can be easily identifiable.  The picture of the physical device will contribute to 10% of the FISA as indicated by the rubric below.  Keep in mind that the marker cannot provide marks for things the marker cannot see.
- Software:  If the layout of the simulation corresponds to the layout of the physical device then the same software can be used for the simulation and the physical device.  Add the code as text to the FISA record.  As shown in the table below, the software contributes 30% of the FISA mark.
- Circuit diagram:  Because of the difference between the drum’s motor for the simulated and physical circuit, you need to draw two circuit diagrams.  For the simulated circuit you have to show four LEDs for the drum’s motor.  No extra electrical connections are required.  For the physical, you have to include all the electrical connections to the motor and the Arduino.
- Calculations: various values have to be calculated as shown in this document and in the rubric.  The calculations, together with the indication that renewable energies are used, contribute 2% to the FISA mark.
- Presentation:  Each student will have to make a PowerPoint presentation that is less than 7 minutes in length of their projects.  Programs similar in function to PowerPoint can also be used instead of PowerPoint.  During this time, the marker will ask a question.  If students get the question 100% then the marker ask the following students another question.  If students do not get the question 100% correct then the marker ask the following group the same question.  The presentation will contribute 10% to the FISA mark.
- Demonstration of simulation:  You will need to show the lecturer how the simulation reacts.  Use a computer only to show the simulations.  The student will need to know how to operate all the components of the washing machine.  It contributes 15% to the FISA mark.
- Each student has to hand in a unique project.  If there are parts of the project that is similar to another student’s project, then it will be considered as plagiarism and appropriate steps should be taken.

---

## Recommendations

If the correct equipment is not available, then the following can be used instead:

- An LED can be used to show the operation of a valve.  If the LED is on then the valve is open.
- Assume the solar system provide 5 volt DC and that your whole system works with 5V DC.
