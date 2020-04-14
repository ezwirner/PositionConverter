#include <stdio.h>
#include <math.h>

// Decimal locations are +/- latitude and longitude where + means North
// latitude and East Longitude
struct DecimalPosition {
    double latitude;
    double longitude;
};

// For DMS positions, this is direction. Only half of the following values 
// apply depending on whether it's a longitude or a latitude.
enum Direction {
    NORTH,      // applies to latitude
    SOUTH,      // applies to latitude
    EAST,       // applies to longitude
    WEST        // applies to longitude
};

// For DMS, there's a degrees, minutes and seconds, assume each can be a 
// floating-point value and also include a direction
struct DMSValue {
    double degrees;
    double minutes;
    double seconds;
    Direction direction;
};

// This is the complete DMS location, which contains a latitude and a longitude
struct DMSPosition {
    DMSValue latitude;
    DMSValue longitude;
};

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
    if (decimal.latitude > 90.0 || decimal.latitude < -90.0) {
        return false;
    }
    if (decimal.longitude > 180.0 || decimal.longitude < -180.0) {
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

void printDMS(DMSPosition dms) {
    printf("Latitude = %g %g\' %g\" %s, Longitude = %g %g\' %g\" %s\n",
        dms.latitude.degrees, dms.latitude.minutes, dms.latitude.seconds,
        dms.latitude.direction == NORTH ? "N" : "S",
        dms.longitude.degrees, dms.longitude.minutes, dms.longitude.seconds,
        dms.longitude.direction == EAST ? "E" : "W");
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

int main(int argc, char *argv[]) {

    if (testDecToDMS1() == true) {
        printf("DecToDMS Test 1 PASSED\n");
    } else {
        printf("DecToDMS Test 1 FAILED\n");
    }

    return 0;
}