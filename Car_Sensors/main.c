#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "operations.h"
#include "structs.h"

int main(int argc, char const **argv)
{
	DIE(argc != 2, "Wrong number of arguments\n");

	FILE *in = fopen(argv[1], "rb");

	DIE(!in, "Failed to open the file.\n");

	int number_of_sensors;
	fread(&number_of_sensors, sizeof(int), 1, in);

	sensor *sensors = (sensor *)malloc(number_of_sensors * sizeof(sensor));
	DIE(!sensors, "Failed to malloc.\n");
	for (int i = 0; i < number_of_sensors; i++) {
		int type;
		fread(&type, sizeof(int), 1, in);
		if (type == 1) {  // PMU type
			sensors[i].sensor_type = PMU;
			power_management_unit pmu;
			fread(&pmu.voltage, sizeof(float), 1, in);
			fread(&pmu.current, sizeof(float), 1, in);
			fread(&pmu.power_consumption, sizeof(float), 1, in);
			fread(&pmu.energy_regen, sizeof(int), 1, in);
			fread(&pmu.energy_storage, sizeof(int), 1, in);
			sensors[i].sensor_data = malloc(sizeof(power_management_unit));
			DIE(!sensors[i].sensor_data, "Failed to malloc.\n");
			memcpy(sensors[i].sensor_data, &pmu, sizeof(power_management_unit));
		} else if (type == 0) {  // TIRE type
			sensors[i].sensor_type = TIRE;
			tire_sensor t;
			fread(&t.pressure, sizeof(float), 1, in);
			fread(&t.temperature, sizeof(float), 1, in);
			fread(&t.wear_level, sizeof(int), 1, in);
			fread(&t.performace_score, sizeof(int), 1, in);
			sensors[i].sensor_data = malloc(sizeof(tire_sensor));
			DIE(!sensors[i].sensor_data, "Failed to malloc.\n");
			memcpy(sensors[i].sensor_data, &t, sizeof(tire_sensor));
		}
		fread(&sensors[i].nr_operations, sizeof(int), 1, in);
		sensors[i].operations_idxs =
			malloc(sensors[i].nr_operations * sizeof(int));
		DIE(!sensors[i].operations_idxs, "Failed to malloc.\n");
		for (int j = 0; j < sensors[i].nr_operations; j++)
			fread(&sensors[i].operations_idxs[j], sizeof(int), 1, in);
	}
	fclose(in);
	sort_priority(&sensors, number_of_sensors);
	program(sensors, number_of_sensors);
	return 0;
}
