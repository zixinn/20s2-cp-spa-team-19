#pragma once

#include "../PKB/PKB.h"
#include "Clause.h"
#include "QueryUtility.h"

// Helper class to evaluate Affects
class AffectsBipEvaluator {
 public:
  AffectsBipEvaluator();

  static bool evaluate(unordered_map<STRING, STRING> declarations,
                       Clause clause,
                       unordered_map<STRING, vector<int>>& tempResults);

  ~AffectsBipEvaluator();
};
