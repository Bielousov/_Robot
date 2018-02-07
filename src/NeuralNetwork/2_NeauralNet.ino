void initializeNeuralNetwork() {  
  NeuralNet neuralNet = NeuralNet(InputNodes, HiddenNodes, OutputNodes, &Serial);

  neuralNet.InitialWeightMax = 0.2;
  neuralNet.LearningRate = 0.1;
  neuralNet.Momentum = 0.5;
  neuralNet.ReportFrequencey = 100;
  neuralNet.MaxTrainCycles = 2000;
  
  // Train network
  neuralNet.train((uint8_t*)TrainData, (float*) TargetData, PatternCount, Success);

  Serial.println();
  Serial.println();
  Serial.println("===== Test =====");

  for (uint8_t i = 0; i < sizeof(TestData); i++) {
    neuralNet.getActivation((uint8_t*) TestData[i]);
    Serial.println("-----");
  }
}
