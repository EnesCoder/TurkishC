#include "json/json.h"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <json/reader.h>
#include <json/value.h>
#include <string>
#include <vector>
#include <cctype>

const std::string DELIMETERS = " \n";
const std::string PARENTHESES = "{[(";
const std::string DICT_DIR = "/home/erch/Desktop/CPlusCPlusPlus/TurkishC/dictionary.json";

std::vector<std::string> GetTokens(std::string contents) {
  std::vector<std::string> tokens;
  std::string curTok;
  for (const char c : contents) {
    curTok += c;
    if (DELIMETERS.find(c) != std::string::npos) {
      tokens.push_back(curTok);
      curTok = "";
    } else if (PARENTHESES.find(c) != std::string::npos) {
      curTok += ' ';
      tokens.push_back(curTok);
      curTok = "";
    }
  }

  return tokens;
}

std::string ReadFile(std::string fileName) {
  std::ifstream input;
  input.open(fileName, std::ios::binary);
  if (input && input.is_open()) {
    std::string contents;
    input.seekg(0, std::ios::end);
    contents.resize(input.tellg());
    input.seekg(0, std::ios::beg);
    input.read(&contents[0], contents.size());
    //std::cout << "File Size: " << contents.size() << " bytes" << std::endl;
    //std::cout << GetTcTokens(contents)[1] << std::endl;
    input.close();
    return contents;
  }
  std::cerr << "Could not open .tc file" << std::endl;
  exit(1);
}

void TranspileTC(std::string tcFileName, std::string cFileName) {
  std::string tcContents = ReadFile(tcFileName);
  std::vector<std::string> tokens = GetTokens(tcContents);

  // parse the dictionary json
  std::string tcDictConts = ReadFile(DICT_DIR);
  Json::CharReaderBuilder readerBuilder;
  Json::CharReader *reader = readerBuilder.newCharReader();
  Json::Value tcDictObj;
  Json::String parseErs;
  if(!reader->parse(tcDictConts.c_str(), tcDictConts.c_str() + tcDictConts.size(), &tcDictObj, &parseErs)) {
    std::cerr << "Error parsing the dictionary: " << parseErs << std::endl;
    exit(1);
  }

  //std::cout << tcDictObj["icin"].asString() << std::endl;

  // transpile the actual thing
  std::string translatedC;
  for (std::string token : tokens) { // tokens are turkish
    // get the sub token ready to be translated
    std::string subTok; // the token, but without the special characters, only alpha
    for (const char c : token) {
      if (!std::isalpha(c))
        continue;
      subTok += c;
    }

    // translate it and add it to the translation
    if (tcDictObj.isMember(subTok)){
      std::string translatedTok = tcDictObj[subTok].asString();
      std::string lengthenedToken = token;
      int gap = subTok.length() - translatedTok.length();
      if (gap > 0)
        lengthenedToken.erase(lengthenedToken.size() - (token.size() - subTok.size()) - gap - 1, gap);
      else if (gap < 0)
        lengthenedToken.insert(lengthenedToken.find(subTok) + subTok.length(), " ", std::abs(gap));
      //std::cout << lengthenedToken << std::endl;
      translatedC += lengthenedToken.replace(token.find(subTok), translatedTok.length(), translatedTok);
    } else
      translatedC += token;
    //std::cout << translatedC << std::endl;
  }

  // FINALLY output the transpiled code
  std::ofstream output(cFileName);
  if (output && output.is_open()) {
    output << translatedC << std::endl;
    output.close();
    std::cout << "Successfully transpiled Turkish C, output at: " << cFileName << std::endl;
  } else {
    std::cerr << "Could not open output file" << std::endl;
    exit(1);
  }
  //std::cout << translatedC << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc == 3)
    TranspileTC(argv[1], argv[2]);
  else {
    std::cerr << "You did not provide an output file" << std::endl;
    exit(1);
  }

  return 0;
}
