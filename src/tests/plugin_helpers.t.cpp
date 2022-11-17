//
// Created by guillaume on 25/01/2022.
//
#include <boost/algorithm/string/predicate.hpp>

#include "gtest/gtest.h"
#include "plugin_helpers.hpp"

TEST(plugin_helpers,
     plugindir) {  // Will fail if cigarfilter_config cannot be found in PATH
  auto plugin_dir = cf::get_plugin_dir();
  auto expected = true;
  auto actual = boost::algorithm::ends_with(plugin_dir, "plugins");
  ASSERT_EQ(expected, actual);
}

TEST(plugin_helpers, get_default_plugin) {
  auto success = true;
  try {
    auto plugin = cf::get_plugin(
        "default");  // Throws a boost exception in case of failure
  } catch (const std::exception& e) {
    success = false;
  }
  ASSERT_EQ(success, true);
}