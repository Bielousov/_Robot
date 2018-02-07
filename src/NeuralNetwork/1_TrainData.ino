const float Success = 0.001;
const uint8_t PatternCount = 10;

PROGMEM const uint8_t TrainData[PatternCount][InputNodes] = {
  {0, 0, 0, 0, 0, 0 },
  {64, 0, 0, 0, 0, 0 },
  {128, 0, 0, 0, 0, 0 },
  {128, 128, 0, 0, 0, 0 },
  {128, 255, 0, 0, 0, 0 },
  {255, 255, 0, 0, 0, 0 },
  {255, 255, 128, 0, 0, 0 },
  {255, 255, 255, 0, 0, 0 },
  {0, 0, 0, 255, 255, 255 },
  {255, 255, 255, 255, 255, 255 },
}; 

PROGMEM const float TargetData[PatternCount][OutputNodes] = {
  { 0, 0},
  { 0.1, 0},
  { 0.2, 0},
  { 0.4, 0},
  { 0.6, 0},
  { 0.8, 0},
  { 0.9, 0},
  { 1.0, 0},
  { 0, 1},
  { 1, 1}
};


const uint8_t TestData[][InputNodes] = {
  {64, 128, 255, 0, 0, 0 },
  {128, 128, 0, 255, 0, 0 },
  {128, 128, 0, 0, 255, 255 }
}; 
