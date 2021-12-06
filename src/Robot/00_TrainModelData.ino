const uint8_t TrainDataSize = 7;
PROGMEM const float TrainData[TrainDataSize][InputNodes] = {
  { 1, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 1 },
  { 0, 1, 0, 1 },
  { 0, 1, 1, 1 },
  { 1, 1, 1, 1 },
  { 1, 0, 0, 1 }
}; 

PROGMEM const float TargetData[TrainDataSize][OutputNodes] = {
  { 0, 1, 0, 0, 0 },  
  { 0, 0, 0, 0, 1.0 }, 
  { 0, 0, 0.5, 0, 0 }, 
  { 0, 0, 0, 0.5, 0 }, 
  { 0.5, 0, 0, 0, 0 }, 
  { 1, 0, 0, 0, 0 }, 
  { 0, 0.5, 0, 0, 0 }
};


const uint8_t TestDataSize = 6;
PROGMEM const float TestData[TestDataSize][InputNodes] = {
  { 0, 0.75, 0.66, 0 },
  { 0, 0.2, 0.9, 0.9 },
  { 0, 0.8, 0.2, 0.8 },
  { 0, 0.8, 0.5, 0.2 },
  { 1, 0.1, 0.5, 0.2 },
  { 1, 0.01, 0.01, 0.2 }  
};
