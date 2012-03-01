EESchema Schematic File Version 2  date 01-03-2012 10:07:01
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:con-usb
LIBS:max1811
LIBS:Nintendo_wireless_controller_curcuit-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "1 mar 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 8500 800  0    60   ~ 0
Voltage regulation: 5V -> 3v3\n
Text Notes 6650 3050 0    60   ~ 0
To wireless module\n
Text Notes 2950 6050 0    60   ~ 0
Charging curcuit:\n
Text Notes 5400 6250 0    60   ~ 0
Battery_connector:
Connection ~ 2150 6450
Wire Wire Line
	2050 6450 2150 6450
Wire Wire Line
	2700 6400 2700 5350
Wire Wire Line
	2150 5900 2150 5950
Connection ~ 4450 6700
Wire Wire Line
	3900 6700 4450 6700
Wire Wire Line
	4450 7150 4450 7200
Wire Wire Line
	4000 6350 4000 6400
Wire Wire Line
	4000 6400 3900 6400
Connection ~ 2550 7000
Wire Wire Line
	2700 7000 2550 7000
Wire Wire Line
	2550 7100 2550 7000
Wire Wire Line
	2550 7000 2550 6800
Wire Wire Line
	2550 6800 2550 6700
Wire Wire Line
	2550 6700 2700 6700
Connection ~ 5700 6600
Wire Wire Line
	5350 6600 5700 6600
Wire Wire Line
	5700 7150 5700 7000
Wire Wire Line
	5800 6900 5700 6900
Wire Wire Line
	1500 5700 1500 5350
Wire Wire Line
	1500 5350 1300 5350
Wire Wire Line
	3300 1700 3300 2050
Connection ~ 2400 2900
Wire Wire Line
	2300 2900 2400 2900
Wire Wire Line
	2400 2900 3200 2900
Wire Wire Line
	3200 2900 3200 2550
Wire Wire Line
	3200 2550 3300 2550
Wire Wire Line
	2400 2800 2400 2900
Connection ~ 1750 2900
Wire Wire Line
	1750 3000 1750 2900
Wire Wire Line
	1750 2900 1750 2100
Wire Wire Line
	1750 2100 1900 2100
Wire Wire Line
	1900 2900 1750 2900
Wire Wire Line
	2400 2200 2400 2100
Wire Wire Line
	3300 2350 3200 2350
Wire Wire Line
	3200 2350 3200 2100
Wire Wire Line
	3200 2100 2400 2100
Wire Wire Line
	2400 2100 2300 2100
Connection ~ 2400 2100
Wire Wire Line
	1300 5450 1750 5450
Wire Wire Line
	1750 5450 1750 5300
Wire Wire Line
	5800 6700 5700 6700
Wire Wire Line
	5700 6700 5700 6600
Wire Wire Line
	5700 6600 5700 6500
Wire Wire Line
	5350 7000 5700 7000
Wire Wire Line
	5700 7000 5700 7000
Wire Wire Line
	5700 7000 5700 6900
Connection ~ 5700 7000
Wire Wire Line
	2700 6800 2550 6800
Connection ~ 2550 6800
Wire Wire Line
	4000 7100 4000 7000
Connection ~ 4000 7000
Wire Wire Line
	4000 7000 3900 7000
Wire Wire Line
	4450 6600 4450 6700
Wire Wire Line
	4450 6700 4450 6750
Wire Wire Line
	2150 5300 2150 5350
Wire Wire Line
	2150 5350 2150 5400
Wire Wire Line
	2150 6350 2150 6450
Wire Wire Line
	2150 6450 2150 6500
Wire Wire Line
	2150 6500 2700 6500
Wire Wire Line
	2700 5350 2150 5350
Connection ~ 2150 5350
Text GLabel 2050 6450 0    60   Input ~ 0
CHARGE_INDICATOR
$Comp
L LED D?
U 1 1 4F4F3974
P 2150 6150
F 0 "D?" H 2150 6250 50  0000 C CNN
F 1 "LED" H 2150 6050 50  0000 C CNN
	1    2150 6150
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 4F4F3966
P 2150 5650
F 0 "R?" V 2230 5650 50  0000 C CNN
F 1 "200" V 2150 5650 50  0000 C CNN
	1    2150 5650
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 4F4F395E
P 2150 5300
F 0 "#PWR?" H 2150 5390 20  0001 C CNN
F 1 "+5V" H 2150 5390 30  0000 C CNN
	1    2150 5300
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 4F4F393F
P 4450 6600
F 0 "#PWR?" H 4450 6690 20  0001 C CNN
F 1 "+5V" H 4450 6690 30  0000 C CNN
	1    4450 6600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F4F3934
P 4450 7200
F 0 "#PWR?" H 4450 7200 30  0001 C CNN
F 1 "GND" H 4450 7130 30  0001 C CNN
	1    4450 7200
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR?
U 1 1 4F4F390D
P 4000 6350
F 0 "#PWR?" H 4000 6300 20  0001 C CNN
F 1 "+BATT" H 4000 6450 30  0000 C CNN
	1    4000 6350
	1    0    0    -1  
$EndComp
$Comp
L CP1 C?
U 1 1 4F4F38FC
P 4450 6950
F 0 "C?" H 4500 7050 50  0000 L CNN
F 1 "4.7uF" H 4500 6850 50  0000 L CNN
	1    4450 6950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F4F38CA
P 4000 7100
F 0 "#PWR?" H 4000 7100 30  0001 C CNN
F 1 "GND" H 4000 7030 30  0001 C CNN
	1    4000 7100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F4F38C1
P 2550 7100
F 0 "#PWR?" H 2550 7100 30  0001 C CNN
F 1 "GND" H 2550 7030 30  0001 C CNN
	1    2550 7100
	1    0    0    -1  
$EndComp
$Comp
L MAX1811 ???
U 1 1 4F4F38B3
P 3300 6700
F 0 "???" H 3300 6700 60  0000 C CNN
F 1 "MAX1811" H 3300 6700 60  0000 C CNN
	1    3300 6700
	1    0    0    -1  
$EndComp
$Comp
L CP1 C?
U 1 1 4F4F37F8
P 5350 6800
F 0 "C?" H 5400 6900 50  0000 L CNN
F 1 "4.7uF" H 5400 6700 50  0000 L CNN
	1    5350 6800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F4F372C
P 5700 7150
F 0 "#PWR?" H 5700 7150 30  0001 C CNN
F 1 "GND" H 5700 7080 30  0001 C CNN
	1    5700 7150
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR?
U 1 1 4F4F3725
P 5700 6500
F 0 "#PWR?" H 5700 6450 20  0001 C CNN
F 1 "+BATT" H 5700 6600 30  0000 C CNN
	1    5700 6500
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 J2
U 1 1 4F4F3715
P 6150 6800
F 0 "J2" V 6100 6800 40  0000 C CNN
F 1 "BATT" V 6200 6800 40  0000 C CNN
	1    6150 6800
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR?
U 1 1 4F4F36FD
P 8400 1100
F 0 "#PWR?" H 8400 1050 20  0001 C CNN
F 1 "+BATT" H 8400 1200 30  0000 C CNN
	1    8400 1100
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 4F4F36D4
P 10100 1100
F 0 "#PWR?" H 10100 1200 30  0001 C CNN
F 1 "VCC" H 10100 1200 30  0000 C CNN
	1    10100 1100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 4F4F36B2
P 1500 5700
F 0 "#PWR?" H 1500 5700 30  0001 C CNN
F 1 "GND" H 1500 5630 30  0001 C CNN
	1    1500 5700
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 4F4F36A7
P 1750 5300
F 0 "#PWR?" H 1750 5390 20  0001 C CNN
F 1 "+5V" H 1750 5390 30  0000 C CNN
	1    1750 5300
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 4F4F35BC
P 3300 1050
F 0 "#PWR?" H 3300 1150 30  0001 C CNN
F 1 "VCC" H 3300 1150 30  0000 C CNN
	1    3300 1050
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 4F4F35A9
P 3300 1450
F 0 "R?" V 3380 1450 50  0000 C CNN
F 1 "R" V 3300 1450 50  0000 C CNN
	1    3300 1450
	-1   0    0    1   
$EndComp
$Comp
L C C?
U 1 1 4F4F356F
P 2100 2900
F 0 "C?" H 2150 3000 50  0000 L CNN
F 1 "C" H 2150 2800 50  0000 L CNN
	1    2100 2900
	0    -1   -1   0   
$EndComp
$Comp
L C C?
U 1 1 4F4F3569
P 2100 2100
F 0 "C?" H 2150 2200 50  0000 L CNN
F 1 "C" H 2150 2000 50  0000 L CNN
	1    2100 2100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4F4F3550
P 1750 3000
F 0 "#PWR?" H 1750 3000 30  0001 C CNN
F 1 "GND" H 1750 2930 30  0001 C CNN
	1    1750 3000
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X1
U 1 1 4F4F350C
P 2400 2500
F 0 "X1" H 2400 2650 60  0000 C CNN
F 1 "8MHz" H 2400 2350 60  0000 C CNN
	1    2400 2500
	0    -1   -1   0   
$EndComp
$Comp
L ATTINY2313-S IC1
U 1 1 4F4F34B3
P 4450 2850
F 0 "IC1" H 3600 3800 60  0000 C CNN
F 1 "ATTINY2313-S" H 5050 2000 60  0000 C CNN
F 2 "SO20" H 3650 2000 60  0001 C CNN
	1    4450 2850
	1    0    0    -1  
$EndComp
$Comp
L CONN_4X2 J1
U 1 1 4F4F3414
P 7050 3500
F 0 "J1" H 7050 3750 50  0000 C CNN
F 1 "NRF_conn" V 7050 3500 40  0000 C CNN
	1    7050 3500
	1    0    0    -1  
$EndComp
$Comp
L USB-MB-S X1
U 1 1 4F4F3355
P 1200 5350
F 0 "X1" H 1200 5350 50  0000 C CNN
F 1 "USB-MB-S" H 1250 5150 50  0000 C CNN
F 2 "con-usb-USB-MB-S" H 1200 5250 50  0001 C CNN
	1    1200 5350
	-1   0    0    1   
$EndComp
$EndSCHEMATC
