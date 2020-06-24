/*
  Copyright © 2014 Jesse 'Jeaye' Wilkerson
  See licensing in LICENSE file, or at:
    http://www.opensource.org/licenses/MIT

  File: main.cpp
  Author: Jesse 'Jeaye' Wilkerson
*/

#include <exception>
#include <iostream>

#include "cmdline.hpp"
#include "groff.hpp"
#include "html.hpp"
#include "op.hpp"
#include "parse_state.hpp"
#include "render.hpp"

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  try {
    parse_state state;

    op<cmdline>(state, argc, argv);
    op<html>(state);
    op<render>(state);
    op<groff>(state);
  } catch (helper const &h) {
    h.help();
  } catch (early_exit const &) {
  } catch (std::exception const &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "unknown error" << std::endl;
    return EXIT_FAILURE;
  }
}
