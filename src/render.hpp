/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing in LICENSE file, or at:
    http://www.opensource.org/licenses/MIT

  File: render.hpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#pragma once

#include <glog/logging.h>
#include <unistd.h>

#include <array>
#include <memory>
#include <string>

#include "parse_state.hpp"
#include "str.hpp"

class render {
 public:
  void operator()(parse_state &state) {
    /* Render the stripped html to plain text. */
    const std::string part_cmd =
        "elinks -dump 1 -dump-width 90 -no-numbering -no-references "
        "-dump-charset UTF-8 -force-html ";
    state.plain = execute(part_cmd + state.tmp_file);

    DLOG(INFO) << "Execute elinks for file:" << state.tmp_file
               << ", get result:\n"
               << state.plain;

    /* Cleanup a bit. */
    str::replace(state.plain, "[edit]", "");
    str::replace(state.plain, "Run this code", "\n// Run this code");
    str::replace(state.plain, "\u200b",
                 ""); /* I don't know why these show up... */
    str::replace(state.plain, "•", "*");
    str::replace(state.plain, "\u00a0", " "); /* NBSP sucks */
    str::replace(state.plain, "(constructor)", "constructor  ");
    str::replace(state.plain, "(destructor)", "destructor  ");
    str::replace(state.plain, "\\", "\\\\");
    DLOG(INFO) << "After cleanup:\n" << state.plain;
    DLOG(INFO) << "Now runnig elinks for original file:" << state.input_file
               << ", get result:\n"
               << execute(part_cmd + state.input_file);
  }

 private:
  static std::string execute(std::string const &cmd) {
    std::unique_ptr<FILE, decltype(&pclose)> const pipe{popen(cmd.c_str(), "r"),
                                                        &pclose};
    if (!pipe) {
      throw std::runtime_error("failed to execute command: " + cmd);
    }

    std::string output;
    char *res{};
    std::array<char, 2048> data;
    while ((res = ::fgets(data.data(), data.size(), pipe.get()))) {
      output += res;
    }
    return output;
  }
};
