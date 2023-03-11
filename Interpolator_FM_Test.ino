/*
                        Interpolator Accum Test
                        Vernon Billingsley c2023
                          Raspberry Pi Pico
                         Arduino IDE 1.8.13

                  Setup the interpolator and draw sine wave modulated
                  by a second sine wave to the serial plotter...
                  
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission
    notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

                Raspberry Pi Pico

                -----------------
               |      |USB|      |
  TXD 0 GPIO0 -|1              40|-  VBUS
  RXD 0 GPIO1 -|2              39|-  Vsys
        GND   -|3              38|-  GND
        GPIO2 -|4              37|-  EN
        GPIO3 -|5              36|-  3.3v Out
        GPIO4 -|6              35|-  ADC Ref
        GPIO5 -|7              34|-  GPIO28  ADC2
        GND   -|8              33|-  GND AGND
        GPIO6 -|9              32|-  GPIO27  ACD1
        GPIO7 -|10             31|-  GPIO26
        GPIO8 -|11             30|-  Run/Reset
        GPIO9 -|12             29|-  GPIO22
        GND   -|13             28|-  GND
       GPIO10 -|14             27|-  GPIO21
       GPIO11 -|15             26|-  GPIO20
       GPIO12 -|16             25|-  GPIO19
       GPIO13 -|17             24|-  GPIO18
        GND   -|18             23|-  GND
       GPIO14 -|19             22|-  GPIO17
       GPIO15 -|20             21|-  GPIO16
                -----------------

  Board json..
  https://github.com/earlephilhower/arduino-pico

  Board Settings:
  Raspberry Pi Pico
  2MB (on FS)
  133 mHz
  Small (-Os) (standard)
  RTTI: disabled
  Stack Protector: disabled
  C++ Exceptions: disabled
  Debug Port: disabled
  Debug Level: none
  USB Stack: "Pico SDK"
  IP Stack: "IPv4 Only"
  Upload Method: "Default (UF2)"

*/
/************************ Includes ********************************/
#include "hardware/interp.h"


/************************* Defines ********************************/
#if !defined(LED_BUILTIN)
#define LED_BUILTIN     25
#endif

#define DEBUG 1

#if DEBUG > 0
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.print( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

/************************** Init Includes *************************/

/************************** Variables *****************************/

short sine_table[2048] = {
  0, 6, 13, 19, 25, 31, 38, 44, 50, 57, 63, 69, 75, 82, 88, 94, 100, 107, 113, 119, 126, 132, 138, 144, 151, 157, 163, 169, 176, 182, 188, 194, 201, 207, 213, 219, 226, 232, 238, 244, 251, 257, 263, 269, 276, 282, 288, 294, 301, 307, 313, 319, 325, 332, 338, 344, 350, 356, 363, 369, 375, 381, 387, 393, 400, 406, 412, 418, 424, 430, 436, 443, 449, 455, 461, 467, 473, 479, 485, 492, 498, 504, 510, 516, 522, 528, 534, 540, 546, 552, 558, 564, 570, 576, 582, 588, 595, 601, 607, 613, 619, 624, 630, 636, 642, 648, 654, 660, 666, 672, 678, 684, 690, 696, 702, 708, 714, 719, 725, 731, 737, 743, 749, 755, 760, 766, 772, 778, 784,
  790, 795, 801, 807, 813, 818, 824, 830, 836, 841, 847, 853, 859, 864, 870, 876, 881, 887, 893, 898, 904, 910, 915, 921, 926, 932, 938, 943, 949, 954, 960, 965, 971, 976, 982, 988, 993, 999, 1004, 1009, 1015, 1020, 1026, 1031, 1037, 1042, 1047, 1053, 1058, 1064, 1069, 1074, 1080, 1085, 1090, 1096, 1101, 1106, 1112, 1117, 1122, 1127, 1133, 1138, 1143, 1148, 1153, 1159, 1164, 1169, 1174, 1179, 1184, 1190, 1195, 1200, 1205, 1210, 1215, 1220, 1225, 1230, 1235, 1240, 1245, 1250, 1255, 1260, 1265, 1270, 1275, 1280, 1285, 1289, 1294, 1299, 1304, 1309, 1314, 1319, 1323, 1328, 1333, 1338, 1342, 1347, 1352, 1357, 1361, 1366, 1371, 1375, 1380, 1385, 1389, 1394, 1398, 1403, 1408, 1412, 1417, 1421, 1426, 1430, 1435, 1439, 1444, 1448, 1453,
  1457, 1461, 1466, 1470, 1475, 1479, 1483, 1488, 1492, 1496, 1500, 1505, 1509, 1513, 1517, 1522, 1526, 1530, 1534, 1538, 1543, 1547, 1551, 1555, 1559, 1563, 1567, 1571, 1575, 1579, 1583, 1587, 1591, 1595, 1599, 1603, 1607, 1611, 1615, 1618, 1622, 1626, 1630, 1634, 1637, 1641, 1645, 1649, 1652, 1656, 1660, 1663, 1667, 1671, 1674, 1678, 1682, 1685, 1689, 1692, 1696, 1699, 1703, 1706, 1710, 1713, 1717, 1720, 1724, 1727, 1730, 1734, 1737, 1740, 1744, 1747, 1750, 1753, 1757, 1760, 1763, 1766, 1769, 1773, 1776, 1779, 1782, 1785, 1788, 1791, 1794, 1797, 1800, 1803, 1806, 1809, 1812, 1815, 1818, 1821, 1824, 1826, 1829, 1832, 1835, 1838, 1840, 1843, 1846, 1849, 1851, 1854, 1857, 1859, 1862, 1865, 1867, 1870, 1872, 1875, 1877, 1880, 1882, 1885, 1887, 1890, 1892, 1895, 1897,
  1899, 1902, 1904, 1906, 1908, 1911, 1913, 1915, 1917, 1920, 1922, 1924, 1926, 1928, 1930, 1932, 1935, 1937, 1939, 1941, 1943, 1945, 1947, 1949, 1950, 1952, 1954, 1956, 1958, 1960, 1962, 1963, 1965, 1967, 1969, 1970, 1972, 1974, 1975, 1977, 1979, 1980, 1982, 1984, 1985, 1987, 1988, 1990, 1991, 1993, 1994, 1995, 1997, 1998, 2000, 2001, 2002, 2004, 2005, 2006, 2007, 2009, 2010, 2011, 2012, 2013, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026, 2027, 2028, 2029, 2029, 2030, 2031, 2032, 2033, 2033, 2034, 2035, 2036, 2036, 2037, 2038, 2038, 2039, 2039, 2040, 2040, 2041, 2041, 2042, 2042, 2043, 2043, 2044, 2044, 2045, 2045, 2045, 2046, 2046, 2046, 2046, 2047, 2047, 2047, 2047, 2047, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048,
  2048, 2048, 2048, 2048, 2047, 2047, 2047, 2047, 2047, 2046, 2046, 2046, 2046, 2045, 2045, 2045, 2044, 2044, 2043, 2043, 2042, 2042, 2041, 2041, 2040, 2040, 2039, 2039, 2038, 2038, 2037, 2036, 2036, 2035, 2034, 2033, 2033, 2032, 2031, 2030, 2029, 2029, 2028, 2027, 2026, 2025, 2024, 2023, 2022, 2021, 2020, 2019, 2018, 2017, 2016, 2015, 2013, 2012, 2011, 2010, 2009, 2007, 2006, 2005, 2004, 2002, 2001, 2000, 1998, 1997, 1995, 1994, 1993, 1991, 1990, 1988, 1987, 1985, 1984, 1982, 1980, 1979, 1977, 1975, 1974, 1972, 1970, 1969, 1967, 1965, 1963, 1962, 1960, 1958, 1956, 1954, 1952, 1950, 1949, 1947, 1945, 1943, 1941, 1939, 1937, 1935, 1932, 1930, 1928, 1926, 1924, 1922, 1920, 1917, 1915, 1913, 1911, 1908, 1906, 1904, 1902, 1899, 1897, 1895, 1892, 1890, 1887, 1885, 1882,
  1880, 1877, 1875, 1872, 1870, 1867, 1865, 1862, 1859, 1857, 1854, 1851, 1849, 1846, 1843, 1840, 1838, 1835, 1832, 1829, 1826, 1824, 1821, 1818, 1815, 1812, 1809, 1806, 1803, 1800, 1797, 1794, 1791, 1788, 1785, 1782, 1779, 1776, 1773, 1769, 1766, 1763, 1760, 1757, 1753, 1750, 1747, 1744, 1740, 1737, 1734, 1730, 1727, 1724, 1720, 1717, 1713, 1710, 1706, 1703, 1699, 1696, 1692, 1689, 1685, 1682, 1678, 1674, 1671, 1667, 1663, 1660, 1656, 1652, 1649, 1645, 1641, 1637, 1634, 1630, 1626, 1622, 1618, 1615, 1611, 1607, 1603, 1599, 1595, 1591, 1587, 1583, 1579, 1575, 1571, 1567, 1563, 1559, 1555, 1551, 1547, 1543, 1538, 1534, 1530, 1526, 1522, 1517, 1513, 1509, 1505, 1500, 1496, 1492, 1488, 1483, 1479, 1475, 1470, 1466, 1461, 1457, 1453, 1448, 1444, 1439, 1435, 1430, 1426,
  1421, 1417, 1412, 1408, 1403, 1398, 1394, 1389, 1385, 1380, 1375, 1371, 1366, 1361, 1357, 1352, 1347, 1342, 1338, 1333, 1328, 1323, 1319, 1314, 1309, 1304, 1299, 1294, 1289, 1285, 1280, 1275, 1270, 1265, 1260, 1255, 1250, 1245, 1240, 1235, 1230, 1225, 1220, 1215, 1210, 1205, 1200, 1195, 1190, 1184, 1179, 1174, 1169, 1164, 1159, 1153, 1148, 1143, 1138, 1133, 1127, 1122, 1117, 1112, 1106, 1101, 1096, 1090, 1085, 1080, 1074, 1069, 1064, 1058, 1053, 1047, 1042, 1037, 1031, 1026, 1020, 1015, 1009, 1004, 999, 993, 988, 982, 976, 971, 965, 960, 954, 949, 943, 938, 932, 926, 921, 915, 910, 904, 898, 893, 887, 881, 876, 870, 864, 859, 853, 847, 841, 836, 830, 824, 818, 813, 807, 801, 795, 790, 784, 778, 772, 766, 760, 755, 749,
  743, 737, 731, 725, 719, 714, 708, 702, 696, 690, 684, 678, 672, 666, 660, 654, 648, 642, 636, 630, 624, 619, 613, 607, 601, 595, 588, 582, 576, 570, 564, 558, 552, 546, 540, 534, 528, 522, 516, 510, 504, 498, 492, 485, 479, 473, 467, 461, 455, 449, 443, 436, 430, 424, 418, 412, 406, 400, 393, 387, 381, 375, 369, 363, 356, 350, 344, 338, 332, 325, 319, 313, 307, 301, 294, 288, 282, 276, 269, 263, 257, 251, 244, 238, 232, 226, 219, 213, 207, 201, 194, 188, 182, 176, 169, 163, 157, 151, 144, 138, 132, 126, 119, 113, 107, 100, 94, 88, 82, 75, 69, 63, 57, 50, 44, 38, 31, 25, 19, 13, 6, 0, -5, -12, -18, -24, -30, -37, -43,
  -49, -56, -62, -68, -74, -81, -87, -93, -99, -106, -112, -118, -125, -131, -137, -143, -150, -156, -162, -168, -175, -181, -187, -193, -200, -206, -212, -218, -225, -231, -237, -243, -250, -256, -262, -268, -275, -281, -287, -293, -300, -306, -312, -318, -324, -331, -337, -343, -349, -355, -362, -368, -374, -380, -386, -392, -399, -405, -411, -417, -423, -429, -435, -442, -448, -454, -460, -466, -472, -478, -484, -491, -497, -503, -509, -515, -521, -527, -533, -539, -545, -551, -557, -563, -569, -575, -581, -587, -594, -600, -606, -612, -618, -623, -629, -635, -641, -647, -653, -659, -665, -671, -677, -683, -689, -695, -701, -707, -713, -718, -724, -730, -736, -742, -748, -754, -759, -765, -771, -777, -783, -789, -794, -800, -806, -812, -817, -823, -829,
  -835, -840, -846, -852, -858, -863, -869, -875, -880, -886, -892, -897, -903, -909, -914, -920, -925, -931, -937, -942, -948, -953, -959, -964, -970, -975, -981, -987, -992, -998, -1003, -1008, -1014, -1019, -1025, -1030, -1036, -1041, -1046, -1052, -1057, -1063, -1068, -1073, -1079, -1084, -1089, -1095, -1100, -1105, -1111, -1116, -1121, -1126, -1132, -1137, -1142, -1147, -1152, -1158, -1163, -1168, -1173, -1178, -1183, -1189, -1194, -1199, -1204, -1209, -1214, -1219, -1224, -1229, -1234, -1239, -1244, -1249, -1254, -1259, -1264, -1269, -1274, -1279, -1284, -1288, -1293, -1298, -1303, -1308, -1313, -1318, -1322, -1327, -1332, -1337, -1341, -1346, -1351, -1356, -1360, -1365, -1370, -1374, -1379, -1384, -1388, -1393, -1397, -1402, -1407, -1411, -1416, -1420, -1425, -1429, -1434, -1438, -1443, -1447, -1452, -1456, -1460, -1465, -1469, -1474, -1478, -1482, -1487,
  -1491, -1495, -1499, -1504, -1508, -1512, -1516, -1521, -1525, -1529, -1533, -1537, -1542, -1546, -1550, -1554, -1558, -1562, -1566, -1570, -1574, -1578, -1582, -1586, -1590, -1594, -1598, -1602, -1606, -1610, -1614, -1617, -1621, -1625, -1629, -1633, -1636, -1640, -1644, -1648, -1651, -1655, -1659, -1662, -1666, -1670, -1673, -1677, -1681, -1684, -1688, -1691, -1695, -1698, -1702, -1705, -1709, -1712, -1716, -1719, -1723, -1726, -1729, -1733, -1736, -1739, -1743, -1746, -1749, -1752, -1756, -1759, -1762, -1765, -1768, -1772, -1775, -1778, -1781, -1784, -1787, -1790, -1793, -1796, -1799, -1802, -1805, -1808, -1811, -1814, -1817, -1820, -1823, -1825, -1828, -1831, -1834, -1837, -1839, -1842, -1845, -1848, -1850, -1853, -1856, -1858, -1861, -1864, -1866, -1869, -1871, -1874, -1876, -1879, -1881, -1884, -1886, -1889, -1891, -1894, -1896, -1898, -1901, -1903, -1905, -1907, -1910, -1912, -1914,
  -1916, -1919, -1921, -1923, -1925, -1927, -1929, -1931, -1934, -1936, -1938, -1940, -1942, -1944, -1946, -1948, -1949, -1951, -1953, -1955, -1957, -1959, -1961, -1962, -1964, -1966, -1968, -1969, -1971, -1973, -1974, -1976, -1978, -1979, -1981, -1983, -1984, -1986, -1987, -1989, -1990, -1992, -1993, -1994, -1996, -1997, -1999, -2000, -2001, -2003, -2004, -2005, -2006, -2008, -2009, -2010, -2011, -2012, -2014, -2015, -2016, -2017, -2018, -2019, -2020, -2021, -2022, -2023, -2024, -2025, -2026, -2027, -2028, -2028, -2029, -2030, -2031, -2032, -2032, -2033, -2034, -2035, -2035, -2036, -2037, -2037, -2038, -2038, -2039, -2039, -2040, -2040, -2041, -2041, -2042, -2042, -2043, -2043, -2044, -2044, -2044, -2045, -2045, -2045, -2045, -2046, -2046, -2046, -2046, -2046, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2047, -2046, -2046, -2046, -2046,
  -2046, -2045, -2045, -2045, -2045, -2044, -2044, -2044, -2043, -2043, -2042, -2042, -2041, -2041, -2040, -2040, -2039, -2039, -2038, -2038, -2037, -2037, -2036, -2035, -2035, -2034, -2033, -2032, -2032, -2031, -2030, -2029, -2028, -2028, -2027, -2026, -2025, -2024, -2023, -2022, -2021, -2020, -2019, -2018, -2017, -2016, -2015, -2014, -2012, -2011, -2010, -2009, -2008, -2006, -2005, -2004, -2003, -2001, -2000, -1999, -1997, -1996, -1994, -1993, -1992, -1990, -1989, -1987, -1986, -1984, -1983, -1981, -1979, -1978, -1976, -1974, -1973, -1971, -1969, -1968, -1966, -1964, -1962, -1961, -1959, -1957, -1955, -1953, -1951, -1949, -1948, -1946, -1944, -1942, -1940, -1938, -1936, -1934, -1931, -1929, -1927, -1925, -1923, -1921, -1919, -1916, -1914, -1912, -1910, -1907, -1905, -1903, -1901, -1898, -1896, -1894, -1891, -1889, -1886, -1884, -1881, -1879, -1876, -1874, -1871, -1869, -1866, -1864, -1861,
  -1858, -1856, -1853, -1850, -1848, -1845, -1842, -1839, -1837, -1834, -1831, -1828, -1825, -1823, -1820, -1817, -1814, -1811, -1808, -1805, -1802, -1799, -1796, -1793, -1790, -1787, -1784, -1781, -1778, -1775, -1772, -1768, -1765, -1762, -1759, -1756, -1752, -1749, -1746, -1743, -1739, -1736, -1733, -1729, -1726, -1723, -1719, -1716, -1712, -1709, -1705, -1702, -1698, -1695, -1691, -1688, -1684, -1681, -1677, -1673, -1670, -1666, -1662, -1659, -1655, -1651, -1648, -1644, -1640, -1636, -1633, -1629, -1625, -1621, -1617, -1614, -1610, -1606, -1602, -1598, -1594, -1590, -1586, -1582, -1578, -1574, -1570, -1566, -1562, -1558, -1554, -1550, -1546, -1542, -1537, -1533, -1529, -1525, -1521, -1516, -1512, -1508, -1504, -1499, -1495, -1491, -1487, -1482, -1478, -1474, -1469, -1465, -1460, -1456, -1452, -1447, -1443, -1438, -1434, -1429, -1425, -1420, -1416, -1411, -1407, -1402, -1397, -1393, -1388,
  -1384, -1379, -1374, -1370, -1365, -1360, -1356, -1351, -1346, -1341, -1337, -1332, -1327, -1322, -1318, -1313, -1308, -1303, -1298, -1293, -1288, -1284, -1279, -1274, -1269, -1264, -1259, -1254, -1249, -1244, -1239, -1234, -1229, -1224, -1219, -1214, -1209, -1204, -1199, -1194, -1189, -1183, -1178, -1173, -1168, -1163, -1158, -1152, -1147, -1142, -1137, -1132, -1126, -1121, -1116, -1111, -1105, -1100, -1095, -1089, -1084, -1079, -1073, -1068, -1063, -1057, -1052, -1046, -1041, -1036, -1030, -1025, -1019, -1014, -1008, -1003, -998, -992, -987, -981, -975, -970, -964, -959, -953, -948, -942, -937, -931, -925, -920, -914, -909, -903, -897, -892, -886, -880, -875, -869, -863, -858, -852, -846, -840, -835, -829, -823, -817, -812, -806, -800, -794, -789, -783, -777, -771, -765, -759, -754, -748, -742, -736, -730, -724, -718, -713, -707, -701,
  -695, -689, -683, -677, -671, -665, -659, -653, -647, -641, -635, -629, -623, -618, -612, -606, -600, -594, -587, -581, -575, -569, -563, -557, -551, -545, -539, -533, -527, -521, -515, -509, -503, -497, -491, -484, -478, -472, -466, -460, -454, -448, -442, -435, -429, -423, -417, -411, -405, -399, -392, -386, -380, -374, -368, -362, -355, -349, -343, -337, -331, -324, -318, -312, -306, -300, -293, -287, -281, -275, -268, -262, -256, -250, -243, -237, -231, -225, -218, -212, -206, -200, -193, -187, -181, -175, -168, -162, -156, -150, -143, -137, -131, -125, -118, -112, -106, -99, -93, -87, -81, -74, -68, -62, -56, -49, -43, -37, -30, -24, -18, -12, -5,
};
/************************** Variables1 ****************************/

/**************************  Functions ****************************/
void interpolator0_setup() {
  /* Config interp0 default , lane 0 */
  interp_config cfg0 = interp_default_config();
  interp_set_config(interp0, 0, &cfg0);
  /* Add base back to accum */
  interp_config_set_add_raw(&cfg0, true);
  /* Set shift for top 11 bits */
  interp_config_set_shift(&cfg0, 21);
  /* Set mask bits */
  interp_config_set_mask(&cfg0, 0, 11);
  /* Update the config */
  interp_set_config(interp0, 0, &cfg0);

}

void interpolator1_setup() {
  /* Config interp1 default , lane 1 */
  interp_config cfg1 = interp_default_config();
  interp_set_config(interp1, 1, &cfg1);
  /* Add base back to accum */
  interp_config_set_add_raw(&cfg1, true);
  /* Set shift for top 11 bits */
  interp_config_set_shift(&cfg1, 21);
  /* Set mask bits */
  interp_config_set_mask(&cfg1, 0, 11);
  /* Update the config */
  interp_set_config(interp1, 1, &cfg1);

}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG > 0) {
    Serial.begin(115200);
    while (!Serial);
    // Serial.print(F(BOARD_NAME)); Serial.print(" @ "); Serial.print(F_CPU); Serial.println(" mHz");
  }

  interpolator0_setup();
  interpolator1_setup();

  /* Clear the accum */
  interp0->accum[0] = 0;
  interp1->accum[0] = 0;
  /* Load the count into  base */
  /* uint32_t aWord = (2^32) * freq / referance clock */
  interp0->base[0] = 10713070;  /* 110 */
  interp1->base[1] = 42852281;  /* 440 */


}/**************************  End Setup **************************/

/******************************************************************/
/*************************** Setup1 *******************************/
/******************************************************************/
void setup1() {



}/**************************  End Setup1 **************************/

/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {
  
  /* Get the modulator step */
  int this_x = interp0->pop[2];
  /* Get the base freq step */
  int this_y = interp1->pop[2];

  /* Get the sine value of the modulator from the table */
  int this_s = (sine_table[this_x]) >> 1;
  /* Apply the modulator to the base step */
  int this_z = (this_y + this_s) & 0x07ff;

  /* Display */
  Serial.println(sine_table[this_z]);

  delay(10);

}/*************************** End Loop *****************************/

/******************************************************************/
/**************************** Loop1 *******************************/
/******************************************************************/
void loop1() {


}/*************************** End Loop1 ****************************/
