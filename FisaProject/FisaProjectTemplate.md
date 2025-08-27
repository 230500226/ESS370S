# 2025 Embedded Systems 3 FISA Memorandum Template
## The design and evaluation of a solar powered washing machine

---

### Template

Some of the information shown on the template are different and some are identical to the template for the GA. The documents provided must be different although it can be similar. If you copy from yourself then it is not considered plagiarism. This template can just be the GA template changed to accommodate everything required here. The part that follows is the template for the record you will need to submit. Read through this template and replace all the statements and questions in green with your own statements. Your statements must address the questions and statements made in the template. Thus, the questions, statements and drawings are in the template to guide you. Delete all the statements in green. It is recommended that you keep the headings.

---

## Cover or Title page

- Name of the practical  
- Subject  
- Name of student  
- Date practical was performed  
- Date report was handed in  
- Declaration that the report is the author’s own work  
- Signature  
- This page must not be numbered  

---

## Abstract

It is recommended that you do this part last. This does not mean that you should put it last in the document. Thus, the abstract must follow the cover page. This part has to contain an overall of everything. This is why it is recommended to do the abstract after the rest of the document is finished. Look at each section in the rest of the document. In one or two sentences, explain what happens in each section. Do this with all the sections starting with the introduction and ending with the conclusion.

---

## Terms of reference

---

## Table of contents

---

## List of Illustrations

Indicate which pictures are on which pages. Pictures that you will need to include is of the simulation and the physical layout.

---

## Glossary of terms

Explain what different abbreviations, which you are going to use, means. For example: D.C means Direct Current.

---

## Introduction

---

## Subject

---

## Background

---

## Problem statement

---

## Objectives

Explain what you are trying to achieve in the end.

---

## Methodology

Explain which steps you have taken in the designing process.

---

## Project and Data Design

---

### Circuit diagrams (25% of FISA)

Because of the differences between the physical circuit and the simulated circuit, you have to draw two circuit diagrams. The remaining 2% is for the records layout and the way in which you mentioned renewable energies in this document.

---

#### Simulated Circuit Diagram (9% of FISA)

The simulated circuit includes four LEDs to indicate the drum’s motor. Because the LEDs draw so little current, it is not needed to add an additional power supply for the simulated drum motor. Design the simulated circuit in such a way that the simulated Arduino does not burn out.

---

#### Physical Circuit Diagram (14% of FISA)

The physical circuit has a physical motor that turns the drum. Because the Arduino cannot provide enough current to run this electrical motor and external power-supply is required. The rest of the circuit diagrams can be identical.

---

## Calculations (1% of FISA)

**Water level**

Calculate the value the Arduino should read to register when the drum is more than 80% full of water.

---

## Photo of Simulated Circuit (10% of FISA)

Take a screenshot of the hardware simulation part of Tinkercad. Paste the screenshot into this position of the documentation.

---

## Photo of Physical Circuit (10% of FISA)

You are required to take a photo of the physical layout of your “washing machine”. Make sure all the connections to all the components are clearly visible. Make sure your student card is visible in the picture as well. This picture has to be added to this position in your documentation.

---

## Program code (30% of FISA)

Copy and paste the code as text into this part. The program should be able to do the following:
- Let the “washing machine” go through all the required cycles. During this time it should control all the necessary hardware.
- Accommodate different power levels. There are two inputs to this part. The one is via the infrared communication and the other is from the specific cycle that the washing machine is busy with. By using these two values this feature should react accordingly.

---

## Findings

### Explanation of findings

Explain what was working and what was not working. Also explain how well did the circuit work. Include a screenshot of the serial monitor for the simulation as well as the physical circuit.

### Rectifications based on findings

During the designing process, a person encounter’s bugs and thing that does not work properly or at all. Indicate what was not working correctly. Then indicate what you have done to resolve this problem.

---

## Conclusion

---

## Scope and Limitations

Indicate what your washing machine will not be able to do. Also, indicate the implication of this limitation.

---

## Recommendation

If a person has to redesign your washing machine, tell them what advice you can give the designer.

---

## Appendices

This part is only needed if you have extra pages that you what to show to the reader.

---

## References

Place all references here. Use Harvard reverencing. Everything that you have read or looked at to make this possible have to be included over here.

---

## Bibliography

---

Student name………………………………………		Student number………………………………………………….

---

| Category          | Description                                             | Examiner | Moderator | Maximum mark |   |
|-------------------|--------------------------------------------------------|----------|-----------|--------------|---|
| Record FISA       | Circuit Simulation Picture                             |          |           | 2            |10 |
|                   | LEDs emulate Direct Drive motor                        |          |           | 2            |   |
|                   | Drum’s “motor” connections to Arduino                  |          |           | 2            |   |
|                   | Water sensor’s connections                             |          |           | 1            |   |
|                   | Valve connections                                      |          |           | 1            |   |
|                   | Pump’s connections                                     |          |           | 1            |   |
|                   | On/pause button                                        |          |           | 1            |   |
|                   |                                                        |          |           |              |   |
|                   | Physical Circuit Picture                               |          |           | 2            |10 |
|                   | Power is connected correctly to drum motor             |          |           | 2            |   |
|                   | Control signal of drum motor correctly connected       |          |           | 1            |   |
|                   | Power is connected correctly to pump                   |          |           | 1            |   |
|                   | Power is connected correctly to Arduino                |          |           | 1            |   |
|                   | Water level sensor                                     |          |           | 1            |   |
|                   | LED for valve                                          |          |           | 1            |   |
|                   | On/pause button                                        |          |           | 1            |   |
|                   |                                                        |          |           |              |   |
| Software          | Drum’s motor moves without library                     |          |           | 2            |30 |
|                   | Machine waits for start button to be pressed           |          |           | 1            |   |
|                   | Drum is filling with water                             |          |           | 2            |   |
|                   | Washing machine pauses when power is not enough        |          |           | 2            |   |
|                   | Washing machine pauses until power is enough.          |          |           | 2            |   |
|                   | Washing machine pauses for different power levels      |          |           | 2            |   |
|                   | Drum is turning                                        |          |           | 2            |   |
|                   | Drum is turning in both directions                     |          |           | 2            |   |
|                   | Seemingly no unnecessary code                          |          |           | 2            |   |
|                   | Two LEDs on every second step                          |          |           | 2            |   |
|                   | One LED on every second step.                          |          |           | 1            |   |
|                   | Emptying the drum                                      |          |           | 2            |   |
|                   | Spinning                                               |          |           | 2            |   |
|                   | All cycles are included                                |          |           | 2            |   |
|                   | Washing machine pauses for power during multiple cycles|          |           | 2            |   |
|                   | The Arduino does not burn out during any of these cycles|         |           | 2            |   |
|                   |                                                        |          |           |              |   |
| Record layout     |                                                        |          |           | 1            |25 |
|                   | Circuit diagram for simulated circuit                  | Drum motor|          | 2            |   |
|                   | pump                                                   |          |           | 1            |   |
|                   | Valve                                                  |          |           | 1            |   |
|                   | Water level sensor                                     |          |           | 1            |   |
|                   | Communication devices                                  |          |           | 2            |   |
|                   | Start button                                           |          |           | 1            |   |
|                   |                                                        |          |           |              |   |
|                   | Circuit diagram for physical circuit                   | Drum motor connections| | 2         |   |
|                   | Drum motor controller                                  |          |           | 1            |   |
|                   | Controller to Arduino connections                      |          |           | 2            |   |
|                   | Power connections for drum motor                       |          |           | 1            |   |
|                   | Pump                                                   |          |           | 1            |   |
|                   | Power connections for pump                             |          |           | 2            |   |
|                   | Start button                                           |          |           | 1            |   |
|                   | valve                                                  |          |           | 1            |   |
|                   | Communication devices                                  |          |           | 2            |   |
|                   | Water level sensor                                     |          |           | 1            |   |
|                   |                                                        |          |           |              |   |
| Renewable energy  |                                                        |          |           | 1            |   |
| Calculations      |                                                        |          |           | 1            |   |

---

| Demonstration     | Simulated hardware layout                              | Machine waits for button to be pressed| | 2 |15 |
|                   | Water valve functioning                                |          |           | 2            |   |
|                   | Student knows when to adjust water level               |          |           | 2            |   |
|                   | Drum is rotating                                       |          |           | 2            |   |
|                   | Pumping                                                |          |           | 2            |   |
|                   | Student know when to adjust water level for all conditions|       |           | 2            |   |
|                   | Student knows how to send available power level        |          |           | 2            |   |
|                   | Demonstrated in designated time                        |          |           | 1            |   |

---

| Presentation      | Timing                                                 |          |           | 1            |10 |
|                   | Readable                                               |          |           | 1            |   |
|                   | Logical                                                |          |           | 3            |   |
|                   | Pictures                                               |          |           | 1            |   |
|                   | Dressed formal                                         |          |           | 1            |   |
|                   | Answers                                                |          |           | 3            |   |

---

| Total             |                                                        |          |           |100           |100|

