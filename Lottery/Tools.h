#pragma once


inline std::string& trim_right_inplace( std::string &s, const std::string &delimiters = " \f\n\r\t\v" )
{
  return s.erase(s.find_last_not_of(delimiters) + 1);
}

inline std::string& trim_left_inplace( std::string &s, const std::string &delimiters = " \f\n\r\t\v" )
{
  return s.erase(0, s.find_first_not_of(delimiters));
}

inline std::string& trim_inplace( std::string &s, const std::string &delimiters = " \f\n\r\t\v" )
{
  return trim_left_inplace(trim_right_inplace(s, delimiters), delimiters);
}

inline bool dbl_IsLessEqual(double d1, double d2, double dAccuracy)
{
  return d1 <= (d2 + dAccuracy);
}
inline bool dbl_IsGreatEqual(double d1, double d2, double dAccuracy)
{
  return d1 >= (d2 - dAccuracy);
}
inline bool dbl_IsEqual(double d1, double d2, double dAccuracy)
{
  return dbl_IsLessEqual(d1, d2, dAccuracy) && dbl_IsGreatEqual(d1, d2, dAccuracy);
}

