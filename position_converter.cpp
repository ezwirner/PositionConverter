#include <stdio.h>
#include <math.h>

// Decimal locations are +/- latitude and longitude where + means North
// latitude and East Longitude
struct DecimalPosition {
    double latitude;
    double longitude;

    DecimalPosition() {
        latitude = 0.0;
        longitude = 0.0;
    }
};

// For DMS positions, this is direction. Only half of the following values 
// apply depending on whether it's a longitude or a latitude.
enum Direction {
    NORTH = 0,      // applies to latitude
    SOUTH = 1,      // applies to latitude
    EAST = 0,       // applies to longitude
    WEST = 1        // applies to longitude
};

// For DMS, there's a degrees, minutes and seconds, assume each can be a 
// floating-point value and also include a direction
struct DMSValue {
    unsigned int degrees;
    unsigned int minutes;
    double seconds;
    Direction direction;

    DMSValue() {
        degrees = 0;
        minutes = 0;
        seconds = 0.0;
        direction = NORTH;
    }
};

// This is the complete DMS location, which contains a latitude and a longitude
struct DMSPosition {
    DMSValue latitude;
    DMSValue longitude;
};

/**
 * @brief Converts DMS position into decimal position.
 * 
 * @param dms The DMS position to be converted.
 * @param decimal The resulting DMS position on success.
 * @return true If the input was valid and the conversion was successful.
 * @return false If the input was not valid.
 */
bool convertDMSToDecimal(DMSPosition dms, DecimalPosition &decimal) {

    // check positions
    if (dms.latitude.direction != NORTH && dms.latitude.direction != SOUTH ||
        dms.longitude.direction != EAST && dms.longitude.direction != WEST) {
        decimal.latitude = 0.0;
        decimal.longitude = 0.0;
        return false;
    }

    // check ranges
    if (dms.latitude.degrees > 90 || dms.longitude.degrees > 180) {
        return false;
    }
    if (dms.latitude.minutes >= 60 || dms.longitude.minutes >= 60) {
        return false;
    }
    if (dms.latitude.seconds >= 60.0 || dms.longitude.seconds >= 60.0) {
        return false;
    }

    double latitude, longitude;

    latitude = (double) dms.latitude.degrees + 
               ((double) dms.latitude.minutes / 60.0) +
               (dms.latitude.seconds / 3600.0);
    if (dms.latitude.direction == SOUTH) {
        latitude *= -1.0;
    }

    longitude = (double) dms.longitude.degrees +
                ((double) dms.longitude.minutes / 60.0) +
                (dms.longitude.seconds / 3600.0);
    if (dms.longitude.direction == WEST) {
        longitude *= -1.0;
    }

    decimal.latitude = latitude;
    decimal.longitude = longitude;

    return true;
}

/**
 * @brief Converts the given decimal position to DMS.
 * 
 * @param decimal The decimal position.
 * @param dms A reference to the DMS position, which is updated on success.
 * @return true If the input was valid and resulted in a successful conversion.
 * @return false If the input was not valid and could not be reasonably 
 *               converted.
 */
bool convertDecimalToDMS(DecimalPosition decimal, DMSPosition &dms) {

    // check the inputs
    if (decimal.latitude > 90.0 || decimal.latitude < -90.0 ||
        decimal.longitude > 180.0 || decimal.longitude < -180.0) {
        dms.latitude.direction = NORTH;
        dms.latitude.degrees = 0;
        dms.latitude.minutes = 0;
        dms.latitude.seconds = 0.0;
        dms.longitude.direction = EAST;
        dms.longitude.degrees = 0;
        dms.longitude.minutes = 0;
        dms.longitude.seconds = 0.0;
        return false;
    }

    // go ahead and determin directions
    Direction latDirection, longDirection;
    latDirection = decimal.latitude >= 0.0 ? NORTH : SOUTH;
    longDirection = decimal.longitude >= 0.0 ? EAST : WEST;

    // get an absolute value of lat and long now that we know direction
    double latitude = fabs(decimal.latitude);
    double longitude = fabs(decimal.longitude);

    // extract DMS for latitude
    double latDegrees = floor(latitude);
    double latMinutes = floor((latitude - latDegrees) * 60.0);
    double latSeconds = (latitude - latDegrees - latMinutes / 60.0) * 3600.0;
    
    // extract DMS for longitude
    double longDegrees = floor(longitude);
    double longMinutes = floor((longitude - longDegrees) * 60.0);
    double longSeconds = (longitude - longDegrees - longMinutes / 60.0) * 3600.0;

    dms.latitude.degrees = latDegrees;
    dms.latitude.minutes = latMinutes;
    dms.latitude.seconds = latSeconds;
    dms.latitude.direction = latDirection;

    dms.longitude.degrees = longDegrees;
    dms.longitude.minutes = longMinutes;
    dms.longitude.seconds = longSeconds;
    dms.longitude.direction = longDirection;

    return true;
}

/**
 * @brief Checks a decimal position to ensure that all elements are in valid
 *        ranges.
 * 
 * @param decimal The decimal position to be checked.
 * @return true True if the entire position is valid.
 * @return false False if any element of the position is not valid.
 */
bool rangeCheckDecimal(DecimalPosition decimal) {

    if (decimal.latitude < -90.0 || decimal.latitude > 90.0) {
        return false;
    }

    if (decimal.longitude < -180.0 || decimal.longitude > 180.0) {
        return false;
    }

    return true;
}

bool rangeCheckDMS(DMSPosition dms) {

    if (dms.latitude.degrees > 90 || dms.longitude.degrees > 90) { 
        return false;
    }

    if (dms.latitude.minutes >= 60 || dms.longitude.minutes >= 60) {
        return false;
    }

    if (dms.latitude.seconds >= 60.0 || dms.longitude.seconds >= 60.0) {
        return false;
    }

    if (dms.latitude.direction != NORTH && dms.latitude.direction != SOUTH) {
        return false;
    }

    if (dms.longitude.direction != EAST && dms.longitude.direction != WEST) {
        return false;
    }

    return true;
}

/**
 * @brief Prints out a DMS value into human readable format.
 * 
 * @param dms The DMSPosition value to be printed.
 */
void printDMS(DMSPosition dms) {
    printf("Latitude = %u %u\' %g\" %s, Longitude = %u %u\' %g\" %s\n",
        dms.latitude.degrees, dms.latitude.minutes, dms.latitude.seconds,
        dms.latitude.direction == NORTH ? "N" : "S",
        dms.longitude.degrees, dms.longitude.minutes, dms.longitude.seconds,
        dms.longitude.direction == EAST ? "E" : "W");
}

/**
 * @brief Prints out a decimal value in human readable format.
 * 
 * @param decimal The DecimalPosition value to be printed.
 */
void printDecimal(DecimalPosition decimal) {
    printf("Latitude = %g, Longitude = %g\n", decimal.latitude, 
        decimal.longitude);
}

bool testDecToDMS1() {

    DecimalPosition dp;
    dp.latitude = 37.44321867;
    dp.longitude = -127.775334;

    DMSPosition dms;

    if (convertDecimalToDMS(dp, dms) == false) {
        return false;
    }

    printDMS(dms);

    return true;
}

bool testDMSToDec1() {

    DMSPosition dms;
    dms.latitude.direction = NORTH;
    dms.latitude.degrees = 62;
    dms.latitude.minutes = 17;
    dms.latitude.seconds = 41.332;
    dms.longitude.direction = EAST;
    dms.longitude.degrees = 124;
    dms.longitude.minutes = 38;
    dms.longitude.seconds = 7.332;

    DecimalPosition decimal;

    if (convertDMSToDecimal(dms, decimal) == false) {
        return false;
    }

    printDecimal(decimal);

    return true;
}

bool runTests() {

    unsigned int fails = 0;

    printf("\n*** RUNNING STATIC TESTS\n\n");

    if (testDecToDMS1() == true) {
        printf("DecToDMS Test 1 PASSED\n");
    } else {
        printf("DecToDMS Test 1 FAILED\n");
        fails++;
    }

    if (testDMSToDec1() == true) {
        printf("DMSToDec Test 1 PASSED\n");
    } else {
        printf("DMSToDec Test 1 FAILED\n");
        fails++;
    }

    return fails == 0 ? true : false;
}

#include <rapidcheck.h>
#include <rapidcheck/state.h>

using namespace rc;

bool runRapidCheckTests() {

    printf("\n*** RUNNING RapidCheck TESTS\n\n");

    rc::check("Checking valid degrees", 
              [](double degrees1, double degrees2) {
                  DMSPosition dms;
                  dms.latitude.degrees = (unsigned int) degrees1;
                  dms.longitude.degrees = (unsigned int) degrees2;
                  DecimalPosition decimal;
                  bool status = convertDMSToDecimal(dms, decimal);
                  printf("Input:  %g, %g\n", degrees1, degrees2);
                  RC_PRE(status == true);
                  printf("Output:  %g, %g\n", decimal.latitude, decimal.longitude);
                  RC_ASSERT(rangeCheckDecimal(decimal) == true);
              });

    return true;
}

int main(int argc, char *argv[]) {

    if (runTests() == false) {
        printf ("TESTS FAILED!\n");
    } else {
        printf("All tests passed.\n");
    }

    if (runRapidCheckTests() == false) {
        printf("RapidCheck TESTS FAILED!\n");
    } else {
        printf("All RapidCheck tests passed.\n");
    }

    return 0;
}