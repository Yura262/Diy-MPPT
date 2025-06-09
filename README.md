# Diy-MPPT
Making DIY buck MPPT converter for small solar panels

To be clear, in 2021 I was 16yo and was going to school

## 2021
I got first solar panel rated 30w (max measured 12w using resistive variable load) and were charging phones/powerbanks using a buck quick charge module with ip6505 IC. Efficiency wasn’t any good and I was getting max 9v@1a and average 5w. 
I started thinking about optimizing the charging process. 
## 2022
I got an idea to use MPPT to power quick charge module to charge phones or large powerbank. Started research on MPPT controllers, found out that commercially available models are expensive and probably won't even work with such low power. On AliExpress there were many boards claiming suitable voltage/current rating and MPPT feature, but close look at them raised question about whether their MPPT feature is the same thing normal MPPT controllers have. And more complicated boards were too expensive to my simple low power setup and still were quite questionable

I decided to look into diy, because chineese boards looked simple. Found some guys explaining mppt theory and doing shenanigans with light bulbs, and found some simple projects I could replicate. During this process, I learned a lot about dc dc converters and how to control them using PWM. And obvious thing I decided to want to do was that I want a synchronous buck dc dc. That half a volt of diode loss at (100%-dutyCycle) sounded just so painful... All projects I saw at time used diode for rectification and some irf**L**z mosfet for switching. Sadly, I haven't got the idea to look up PWM controlled synchronous dc dc's and put away this project (also partially for sane reasons like the worth of bothering for extra watt or two from my panel)
## 2023
I got a second solar panel rated 40w, max I got was around 22w. I connected it in parallel to the first one. Maximum I was able to get out of them when they were set up was 19w-20w while second solo was giving 17-ish watts. It worked pretty well (using ip6505 module again). I was able to be getting 20ah powerbank charged every day. In retrospect, it wasn't great idea, probably bad for powerbanks, and I had to plug them in everyday. But I wanted to squeeze more power again and decided to look up DIY MPPTs again and found "DIY 1kW Arduino MPPT Solar Charge Controller (WiFi ESP32)" by TechBuilder youtube channel. It was too powerful for my panels (20w < 1kw) but it was pretty simple and came with decent code.

Firstly, I put together parts list with my local prices, which added up quickly, but to a reasonable number. Then I began to research which components I can swap. I had an ESP32 wroom dev module, so obvious first step was to adapt schematic to it. I removed 3.3v regulator, led, and usb-ttl programmer.
Then, the most expensive components, mosfets, I started choosing ones with close gate charge and RDS but with lower current. Since open circuit voltage of cold solar panel can reach 22-23v and I wasn't quite sure about my solarpanell setup (possible series or even 2*300w panels at 36v each), I decided to look for 80v mosfets. Then thought a bit and put together a table with 3 configurations: 
1. original, with mosfets changed (also adapted to esp32 dev module) - $40
2. cut off version with cheaper mosfets                              - $25
3. minimal possible working version with 26v/3a mosfets              - $9 
![image](https://github.com/user-attachments/assets/23bf1370-0263-41dd-a312-da8e3e217357)

![image](https://github.com/user-attachments/assets/95282b27-9c4d-48c8-9324-8da7c4dbb2d4)

Spoiler: I ended up with something between version 2 & 3 total price was around $13

### What I decided to cut off:
![image](https://github.com/user-attachments/assets/814ceba6-bfcc-47f8-b5b8-621545a31cd9)
As said - replace oem esp32 with module - less hassle, can be replaced, has space between it and PCB (SD card fits perfectly in there)

LCD, buttons, heat sink - went first, as not critical.
Next was back flow protection. Since my idea was to power qc module, and it had some diodes and USB chargeable devices have some reverse flow protection too, i pretty quickly decided to cut it out. I had some thoughts about getting a gel/ups battery, but they wear out quickly and are expensive. And also I wasn't sure my PV panels' MPP is higher than battery voltage + at least 1v (I haven’t measured it perfectly then, now I know it is usually somewhere between 10.5v and 13v, pushing 1.4-1.6 amp, which is veeeryy weird btw, rated MPP - 18v)


Then I replaced acs712 with ina3221. That is the best change possible
It:
1. does input & output voltage measurement instead of voltage dividers
2. has internal ADC
3. can measure output current as well
4. cheaper than single external ADC
5. uses shunts - more versatility (although some acs712's can do 50a)

Then I decided to focus on low voltage (my current setup) first simplify more

I removed 80v to 12v buck, thinking I could get an AliExpress module to do it later and for now power it externally via USB cable for now 
Powering mosfet driver is done using mt3608, which boosts 5v to 12v.
![image](https://github.com/user-attachments/assets/01a238ce-9fa2-4f68-9bf1-c3cabc33ba9d)

Inductor was roughly calculated using excel table provided by TechBuilder and bought the closest value one from store:

I ended up doing the most cut version described here but with some changes:
1. irfz44n mosfets - 55v 49a - popular, more powerfull & cheap
2. lowered capacitor voltage ratings
3. added tvs diodes for input and output

## Final Schematic:
