#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "MLP/Model/GNN.h"
#include "MLP/Model/fileexecutor.h"
#include "MLP/Model/matrixneuralnet.h"

std::vector<double> letterA(
    {0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   4.,   9.,
     36.,  18.,  0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   7.,
     32.,  52.,  202., 222., 245., 136., 0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   3.,   21.,  95.,  202., 222., 253., 254., 247., 156., 1.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   2.,   2.,   33.,  171., 232., 252., 254., 254., 253., 219.,
     51.,  19.,  0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   33.,  80.,  65.,  164., 251., 254., 254.,
     247., 217., 202., 90.,  1.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   1.,   77.,  128., 232., 252., 251.,
     254., 253., 254., 239., 109., 5.,   4.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   3.,   16.,  10.,  47.,  218.,
     249., 254., 252., 250., 250., 221., 243., 246., 129., 0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   22.,  159.,
     227., 222., 246., 254., 246., 202., 95.,  37.,  37.,  144., 250., 254.,
     215., 4.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     5.,   33.,  159., 247., 254., 254., 254., 209., 127., 34.,  7.,   0.,
     1.,   139., 250., 254., 213., 4.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   3.,   127., 232., 254., 251., 238., 219., 170., 22.,
     0.,   0.,   0.,   0.,   3.,   184., 253., 211., 60.,  0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   20.,  52.,  231., 254., 247., 171.,
     95.,  48.,  21.,  0.,   0.,   0.,   0.,   0.,   4.,   215., 254., 185.,
     26.,  0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   3.,   200., 250.,
     254., 253., 128., 24.,  4.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     21.,  233., 254., 170., 21.,  0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   2.,   170., 250., 254., 254., 224., 172., 110., 6.,   5.,   15.,
     1.,   0.,   0.,   0.,   37.,  249., 252., 84.,  3.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   20.,  95.,  227., 226., 253., 254.,
     249., 217., 217., 213., 96.,  32.,  8.,   0.,   49.,  250., 250., 37.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   7.,
     68.,  64.,  207., 249., 250., 254., 254., 253., 234., 203., 126., 9.,
     82.,  252., 249., 37.,  0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   8.,   37.,  64.,  218., 247., 251.,
     254., 254., 251., 148., 189., 254., 222., 11.,  0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     5.,   51.,  157., 154., 220., 254., 254., 251., 252., 254., 236., 84.,
     22.,  0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   2.,   2.,   22.,  114., 129., 234.,
     254., 254., 254., 251., 232., 95.,  37.,  50.,  80.,  22.,  0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   4.,   7.,   154., 232., 250., 250., 254., 254., 234., 217., 222.,
     231., 154., 2.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   3.,   21.,  37.,  52.,  202.,
     222., 251., 254., 255., 254., 215., 4.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   1.,   32.,  51.,  175., 249., 250., 232., 154., 2.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   7.,   37.,  37.,
     21.,  3.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,   0.,
     0.,   0.,   0.,   0.});
std::vector<double> answerA(26, 0.1);

TEST(mnn, getAnswer) {
  s21::MatrixNeuralNet mnn;
  mnn.recreate(s21::FileExecutor::getWeights("weights/784_150_70_26.txt", 784));
  int ans = mnn.getAnswer(letterA);
  ASSERT_TRUE(ans == 0);
}

TEST(mnn, learn) {
  s21::MatrixNeuralNet mnn;
  mnn.getAnswer(letterA);
  double error = mnn.getError(answerA);
  mnn.training(letterA, answerA);
  mnn.getAnswer(letterA);
  ASSERT_TRUE(mnn.getError(answerA) < error);
}

TEST(gnn, getAnswer) {
  s21::GNN gnn;
  gnn.recreate(s21::FileExecutor::getWeights("weights/784_150_70_26.txt", 784));
  int ans = gnn.getAnswer(letterA);
  ASSERT_TRUE(ans == 0);
}

TEST(gnn, learn) {
  s21::GNN gnn;
  gnn.getAnswer(letterA);
  double error = gnn.getError(answerA);
  gnn.training(letterA, answerA);
  gnn.getAnswer(letterA);
  ASSERT_TRUE(gnn.getError(answerA) < error);
}

int main(int argc, char **argv) {
  answerA[0] = 0.9;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
