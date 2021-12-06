const uint8_t TrainInputNodes = 1;
const uint8_t TrainOutputNodes = 4;

const uint8_t TrainDataSize = 6;
PROGMEM const float TrainData[TrainDataSize][TrainInputNodes] = {
  { 0.0 },
  { 1.0 },
  { 0.8 },
  { 0.6 },
  { 0.4 },
  { 0.2 },
}; 

PROGMEM const float TargetData[TrainDataSize][TrainOutputNodes] = {
  { 0, 1, 0, 0 },
  { 0, 0, 0.1, 0.3},
  { 0, 0, 0.2, 0.2 }, 
  { 0, 0, 0.4, 0.1 }, 
  { 0, 0, 0.5, 0.05 }, 
  { 0.2, 0.3, 0, 0 }, 
};


const uint8_t TestDataSize = 6;
PROGMEM const float TestData[TestDataSize][TrainInputNodes] = {
  { 0 },
  { 0.1 },
  { 0.3 },
  { 0.5 },
  { 0.7 },
  { 0.9 }  
};
