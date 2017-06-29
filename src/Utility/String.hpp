/**
 * @file String.hpp
 * @brief The String class.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017
 * @version 1
 */




#ifndef MATRIXINSPECTOR_UTILITY_STRING_HPP
#define MATRIXINSPECTOR_UTILITY_STRING_HPP




#include <string>
#include <vector>
#include <stdexcept>




namespace MatrixInpsector
{


class String
{
  public:
    /**
     * @brief Split a string using the specified delimiter.
     *
     * @param str The string to split.
     * @param delim The delimiter to split the string.
     *
     * @return A list of substrings.
     */
    static std::vector<std::string> split(
        std::string const & str,
        std::string const & delim) noexcept;


    /**
     * @brief Check if a string contains the substring.
     *
     * @param str The string.
     * @param substr The substring.
     *
     * @return True if the substring exists within the string.
     */
    static bool contains(
        std::string const & str,
        std::string const & substr) noexcept;


    /**
     * @brief Count the number of occurences of the substring within the 
     * string.
     *
     * @param str The string.
     * @param substr The substring.
     *
     * @return The number of instances.
     */
    static size_t count(
        std::string const & str,
        std::string const & substr) noexcept;


    /**
     * @brief Convert a string to lowercase.
     *
     * @param str The string to convert.
     *
     * @return The lower case string.
     */
    static std::string toLower(
        std::string const * str) noexcept;



    /**
     * @brief Add thousands seperator to the given number.
     *
     * @param T The type of number.
     * @param num The number.
     *
     * @return The string represenation.
     */
    template<typename T>
    static std::string addThousandsSeparators(
        T const num)
    {
      std::string base = std::to_string(num);

      // check if we have a decimal
      size_t decPos = base.find('.');
      if (decPos == std::string::npos) {
        decPos = base.length();
      }

      std::vector<char> sep;

      // build chars in reverse
      size_t lastSep = 0;
      for (size_t i = 0; i < base.size(); ++i) {
        size_t j = base.size() - i - 1;
        if (j < decPos) {
          if (base[j] == '-') {
            if (j != 0) {
              throw std::runtime_error(std::string("Invalid number: ") + base);
            }
          } else {
            if (lastSep == 3) {
              // insert seperator
              sep.emplace_back(',');
              lastSep = 0;
            }
            ++lastSep;
          }
        }
        sep.emplace_back(base[j]);
      }

      // build our string in order
      std::string str(sep.size(),'\0');
      for (size_t i = 0; i < sep.size(); ++i) {
        str[sep.size() - i - 1] = sep[i];
      }

      return str;
    }




};




}




#endif
