//
// Created by guillaume on 25/01/2022.
//
#include <iostream>
#include <string>
#include <vector>

#include "CIGAR.hpp"
#include "gtest/gtest.h"

TEST(cigar, t_listdigit) {
  std::vector<int> listdigit = {1, 2, 3, 4};
  auto actual = cf::listdigits_to_number(listdigit);
  size_t expected = 1234;
  ASSERT_EQ(actual, expected);
}

TEST(cigar, t_eq) {
  auto samline = std::string(
      "m54063_170707_180026/4194490/"
      "ccs\t16\tscaffold_001\t795594\t254\t131=\t*\t0\t0\tATGATTT"
      "AAAATAATTATACATATTCATATAAACAATAACTTAAATATTACAATTTAATACTTCTTCACCCTTACTTTT"
      "AGTCATCTAAAAATC"
      "AGTTTAAAATGACAATATTTAAAATCTTGATTTATAA\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
      "~~~~~~~~~~~~~~~"
      "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
      "~~~~~~~~~~~\tRG"
      ":Z:0415fecc\tnp:i:46\trq:f:1\trs:B:i,47,0,0,0,4,0\tsn:B:f,7.26027\tAS:i:"
      "-655\tNM:i:0");

  auto cigar = cf::get_CIGAR(samline);
  auto actual = cigar.eq;
  size_t expected = 131;
  ASSERT_EQ(actual, expected);

  ASSERT_EQ(cigar.as_string, "131=");
}

TEST(cigar, t_empty) {
  auto samline =
      std::string("identifier\tscaffold_001\t795594\t254\t131=\t*\t0\t0\t");
  // In an ideal world this should produce an error; but since there is a "*" in
  // the 5th field, it produces an "empty"
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.empty;
  auto expected = true;
  ASSERT_EQ(actual, expected);

  actual = cigar.error;
  expected = false;
  ASSERT_EQ(actual, expected);

  ASSERT_EQ(cigar.as_string, "*");
}

TEST(cigar, t_S) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51S12=10S\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.S;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);

  ASSERT_EQ(cigar.as_string, "51S12=10S");
}

TEST(cigar, t_D) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51D12=10D\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.D;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);

  ASSERT_EQ(cigar.as_string, "51D12=10D");
}

TEST(cigar, t_I) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51I12=10I\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.I;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);

  ASSERT_EQ(cigar.as_string, "51I12=10I");
}

TEST(cigar, t_X) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51X12=10X\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.X;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);
}

TEST(cigar, t_N) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51N12=10N\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.N;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);
}

TEST(cigar, t_P) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51P12=10P\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.P;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);
}

TEST(cigar, t_H) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51H12=10H\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.H;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);
}

TEST(cigar, t_M) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t51M12=10M\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  auto actual = cigar.M;
  auto expected = 61;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 12;
  ASSERT_EQ(actual, expected);
}

TEST(cigar, complex_1) {
  auto samline = std::string(
      "m54063\t16\tscaffold_001\t795594\t254\t12H2M1X12S51="
      "2D10S50N1D2I1P1X1I1M1=1P11=12S50H\t*\t0\t0\t");
  auto cigar = cf::get_CIGAR(samline);

  //    X 1 1
  //    S 12 10 12
  //    = 51 1 11
  //    D 1 2
  //    I 2 1
  //    P 1 1
  //    H 12 50
  //    N 50
  //    M 2 1

  auto actual = cigar.X;
  auto expected = 2;
  ASSERT_EQ(actual, expected);

  actual = cigar.S;
  expected = 34;
  ASSERT_EQ(actual, expected);

  actual = cigar.eq;
  expected = 63;
  ASSERT_EQ(actual, expected);

  actual = cigar.M;
  expected = 3;
  ASSERT_EQ(actual, expected);

  actual = cigar.N;
  expected = 50;
  ASSERT_EQ(actual, expected);

  actual = cigar.D;
  expected = 3;
  ASSERT_EQ(actual, expected);

  actual = cigar.I;
  expected = 3;
  ASSERT_EQ(actual, expected);

  actual = cigar.P;
  expected = 2;
  ASSERT_EQ(actual, expected);

  actual = cigar.H;
  expected = 62;
  ASSERT_EQ(actual, expected);

  ASSERT_EQ("12H2M1X12S51=2D10S50N1D2I1P1X1I1M1=1P11=12S50H", cigar.as_string);
}