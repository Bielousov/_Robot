void initializeNeuralNetwork() {  
  NeuralNet neuralNet = NeuralNet(InputNodes, HiddenNodes, OutputNodes, &Serial);

  neuralNet.Alpha = 0.02;
  neuralNet.InitialWeightMax = 0.5;
  neuralNet.LearningRate = 0.3;
  neuralNet.Momentum = 0.9;
  neuralNet.Success = 0.001;
  neuralNet.ReportFrequencey = 1000;
  neuralNet.MaxTrainCycles = 65000;
  
  // Train network
  neuralNet.train((uint8_t*)TrainData, (float*) TargetData, PatternCount);

  Serial.println();
  Serial.println();
  Serial.println("===== Test =====");

  for (uint8_t i = 0; i < sizeof(TestData); i++) {
    neuralNet.getActivation((uint8_t*) TestData[i]);
    Serial.println("-----");
  }
}
