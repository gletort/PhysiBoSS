/* 
   MaBoSS (Markov Boolean Stochastic Simulator)
   Copyright (C) 2011-2018 Institut Curie, 26 rue d'Ulm, Paris, France
   
   MaBoSS is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   MaBoSS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Module:
     RandomGenerator.h

   Authors:
     Eric Viara <viara@sysra.com>
     Gautier Stoll <gautier.stoll@curie.fr>
 
   Date:
     January-March 2011
*/

#ifndef _RANDOMGENERATOR_H_
#define _RANDOMGENERATOR_H_

// DO NOT define USE_DUMMY_RANDOM: except for profiling purpose: this flag has been introduced to get an estimation of random number generation time
//#define USE_DUMMY_RANDOM

//#define RANDOM_TRACE
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>

#include "maboss-config.h"

class RandomGenerator {

  static size_t generated_number_count;

 protected:
  static void incrGeneratedNumberCount() {generated_number_count++;}

 public:
  virtual std::string getName() const = 0;

  virtual bool isPseudoRandom() const = 0;

  virtual unsigned int generateUInt32() = 0;

  virtual double generate() = 0;

  virtual void setSeed(int seed) { }

  static size_t getGeneratedNumberCount() {return generated_number_count;}

  virtual ~RandomGenerator() {}
};

class PhysicalRandomGenerator : public RandomGenerator {
  int fd;

 public:
  PhysicalRandomGenerator() {
    fd = open("/dev/urandom", O_RDONLY);
    assert(fd >= 0);
  }

  bool isPseudoRandom() const {
    return false;
  }

  std::string getName() const {
    return "physical";
  }

  unsigned int generateUInt32() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return ~0U/2;
#endif
    unsigned int result;
    int ret = read(fd, &result, sizeof(result));
    assert(ret == sizeof(result));
#ifdef RANDOM_TRACE
    std::cout << result << '\n';
#endif
    return result;
  }

  virtual double generate() {
    double result = ((double)generateUInt32())/~0U; // fixed this 2014-10-17, but I think I added /2 because it did not work
#ifdef RANDOM_TRACE
    std::cout << result << '\n';
#endif
    return result;
  }

  ~PhysicalRandomGenerator() {
    if (fd >= 0) {
      close(fd);
    }
  }
};

class StandardRandomGenerator : public RandomGenerator {

  int seed;
#ifdef HAS_RAND48_T
  drand48_data data;
#endif

 public:
  StandardRandomGenerator(int seed) : seed(seed) {
#ifdef HAS_RAND48_T
    memset(&data, 0, sizeof(data));
    srand48_r(seed, &data);
#elif defined(HAS_RAND48)
    srand48(seed);
#else
    srand(seed);
#endif
  }

  bool isPseudoRandom() const {
    return true;
  }

  std::string getName() const {
    return "standard";
  }

  unsigned int generateUInt32() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return ~0U/2;
#endif
#ifdef HAS_RAND48_T
    long result;
    lrand48_r(&data, &result);
#ifdef RANDOM_TRACE
    std::cout << (unsigned int)result << '\n';
#endif
    return (unsigned int)result;
#elif defined(HAS_RAND48)
    return lrand48();
#else
    return rand();
#endif
  }

  virtual double generate() {
    incrGeneratedNumberCount();
#ifdef USE_DUMMY_RANDOM
    return 0.5;
#endif
#ifdef HAS_RAND48_T
    double result;
    drand48_r(&data, &result);
#ifdef RANDOM_TRACE
    std::cout << result << '\n';
#endif
    return result;
#elif defined(HAS_RAND48)
    return drand48();
#else
    return (double(rand()) / RAND_MAX);
#endif
  }

  virtual void setSeed(int seed) {
    this->seed = seed;
#ifdef HAS_RAND48_T
    srand48_r(seed, &data);
#elif defined(HAS_RAND48)
    srand48(seed);
#else
    srand(seed);
#endif
  }
};

class RandomGeneratorFactory {

public:
  enum Type {
    STANDARD = 1,
    PHYSICAL
  };

private:
  Type type;

public:
  RandomGeneratorFactory(Type type) : type(type) { }

  RandomGenerator* generateRandomGenerator(int seed) const {
    switch(type) {
    case STANDARD:
      return new StandardRandomGenerator(seed);
    case PHYSICAL:
      return new PhysicalRandomGenerator();
    default:
      abort();
      return NULL;
    }
  }

  std::string getName() const {
    switch(type) {
    case STANDARD:
      return "standard";
    case PHYSICAL:
      return "physical";
    default:
      abort();
      return NULL;
    }
  }

  bool isPseudoRandom() const {
    switch(type) {
    case STANDARD:
      return true;
    case PHYSICAL:
      return false;
    default:
      abort();
      return false;
    }
  }

  bool isThreadSafe() const {
    switch(type) {
    case STANDARD:
#ifdef HAS_RAND48_T
      return true;
#else
      return false;
#endif
    case PHYSICAL:
      return true;
    default:
      abort();
      return false;
    }
  }
};

#endif
