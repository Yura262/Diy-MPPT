void resetVariables(){
  secondsElapsed = 0;
  energySavings  = 0; 
  daysRunning    = 0; 
  timeOn         = 0; 
}
void Read_Sensors()
{

  // /////////// TEMPERATURE SENSOR /////////////
  // if (sampleStoreTS <= avgCountTS)
  // { // TEMPERATURE SENSOR - Lite Averaging
  //   TS = TS + analogRead(TempSensor);
  //   sampleStoreTS++;
  // }
  // else
  // {
  //   TS = TS / sampleStoreTS;
  //   TSlog = log(ntcResistance * (4095.00 / TS - 1.00));
  //   temperature = (1.0 / (1.009249522e-03 + 2.378405444e-04 * TSlog + 2.019202697e-07 * TSlog * TSlog * TSlog)) - 273.15;
  //   sampleStoreTS = 0;
  //   TS = 0;
  // }
  //////////////////
  /////////// VOLTAGE & CURRENT SENSORS /////////////
  VSI = 0.0000; // Clear Previous Input Voltage
  VSO = 0.0000; // Clear Previous Output Voltage
  CSI = 0.0000; // Clear Previous Current

  // VOLTAGE SENSOR - Instantenous Averaging
  for (int i = 0; i < avgCountVS; i++)
  {
    VSI = VSI + ina3221.getBusVoltage_V(Sp2_010); // volts inp ads.computeVolts(ads.readADC_SingleEnded(3));
    VSO = VSO + ina3221.getBusVoltage_V(Out3_010);                                   // volts outp ads.computeVolts(ads.readADC_SingleEnded(1));
  }
  voltageInput = (VSI / avgCountVS); // filtered voltage
  voltageOutput = (VSO / avgCountVS);

  // INPUT CURRENT
  // for (int i = 0; i < avgCountCS; i++)
  // {
  //   CSI = CSI + ina3221.getShuntVoltage_mV(Sap1_100);
  // }
  // currentInput = -(CSI / avgCountCS) * 0.1; // filtered current inp
  for (int i = 0; i < avgCountCS; i++)
  {
    CSI = CSI + ina3221.getCurrent_mA(Sp2_010);
  }
  currentInput = (CSI / avgCountCS) * 0.001;
  //
  CSI=0.0000;
  //
  // if (currentInput < 0)
  // {
  //   currentInput = 0.0000;
  // }
  if (voltageOutput <= 0)
  {
    currentOutput = 0.0000;
  }
  else
  {
    for (int i = 0; i < avgCountCS; i++)
    {
      CSI = CSI + ina3221.getCurrent_mA(Out3_010);
    }
    currentOutput = (CSI / avgCountCS) * 0.001;
  }

  // POWER COMPUTATION - Through computation
  powerInput = voltageInput * currentInput;
  powerOutput = voltageInput * currentInput;

  // TIME DEPENDENT SENSOR DATA COMPUTATION
  currentRoutineMillis = millis();
  if (currentRoutineMillis - prevRoutineMillis >= millisRoutineInterval)
  {                                                                           // Run routine every millisRoutineInterval (ms)
    prevRoutineMillis = currentRoutineMillis;                                 // Store previous time
    Wh = Wh + (powerInput / (3600.000 * (1000.000 / millisRoutineInterval))); // Accumulate and compute energy harvested (3600s*(1000/interval))
    kWh = Wh / 1000.000;
    MWh = Wh / 1000000.000;
    daysRunning = timeOn / (86400.000 * (1000.000 / millisRoutineInterval)); // Compute for days running (86400s*(1000/interval))
    timeOn++;                                                                // Increment time counter
  }

  // OTHER DATA
  secondsElapsed = millis() / 1000;                   // Gets the time in seconds since the was turned on  and active
  energySavings = electricalPrice * (Wh / 1000.0000); // Computes the solar energy saving in terms of money (electricity flag rate)
}
