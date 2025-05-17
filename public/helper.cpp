#include "helper.h"

String formatDigit(String input){
  String output = "";
  int len =input.length();

  for (int i = 0; i < len; i++) {
    output +=input[i];
    // Add dash after every 4 digits except the last group
    if ((i + 1) % 4 == 0 && (i + 1) < len) {
      output += '-';
    }
  }

  return output;
}