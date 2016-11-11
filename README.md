# BBBSensor

This is a project for the Beaglebone Black using a Bosch BME280 sensor.
This sensor reads the temperature, barometric pressure and the relative humidity. After reading those values they are stored in a database.

The project is cross compiled for the target using a Linux desktop. 

For the database a PostgreSQL database is used. In order to use these shared libraries they have to be pre-cross-compiled on the desktop. Use the following commands inside `inc/postgresql`:

``./configure --host=arm-linux-gnueabihf CC=/usr/bin/arm-linux-gnueabihf-gcc AR=/usr/bin/arm-linux-gnueabihf-ar CPP=/usr/bin/arm-linux-gnueabihf-cpp --without-openssl --without-readline --without-zlib --disable-spinlocks --prefix=<lib_destination_folder>/libpq```

then go to `inc/postgresql/src/interfaces` and use:

```make```
```make install``` 

