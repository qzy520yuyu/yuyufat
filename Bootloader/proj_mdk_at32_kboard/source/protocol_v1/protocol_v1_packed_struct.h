#ifndef _PROTOCOL_V1_PACKED_STRUCT_H_
#define _PROTOCOL_V1_PACKED_STRUCT_H_

#include <stdint.h>

#ifndef STR_MAX
#define STR_MAX 256
#endif

#pragma pack(1)

typedef struct
{
	uint64_t 	time_us;
	float 		acc_x;
	float 		acc_y;
	float 		acc_z;
	float 		gyr_x;
	float 		gyr_y;
	float 		gyr_z;
	float 		tmp_d;
}packed_imu_t;

typedef struct
{
	uint64_t 	time_us;
	float 		gyr_x;
	float 		gyr_y;
	float 		gyr_z;
}packed_gyr_t;

typedef struct
{
	uint64_t 	time_us;
	float 		acc_x;
	float 		acc_y;
	float 		acc_z;
}packed_acc_t;

typedef struct
{
	uint64_t 	time_us;
	int16_t		gyr_x;
	int16_t		gyr_y;
	int16_t		gyr_z;
}packed_gyr_i16_t;

typedef struct
{
	uint64_t 	time_us;
	int16_t		acc_x;
	int16_t		acc_y;
	int16_t		acc_z;
}packed_acc_i16_t;

typedef struct
{
	uint64_t 	time_us;
	int16_t		gyr_x;
	int16_t		gyr_y;
	int16_t		gyr_z;	
	int16_t		acc_x;
	int16_t		acc_y;
	int16_t		acc_z;
	int16_t		tmp_d;
}packed_imu_i16_t;

typedef struct
{
	uint64_t 	time_us;
	float 		press;
	float 		tempreture;
	float 		altitude;
}packed_baro_t;

typedef struct
{
	uint64_t 	time_us;
	float 		mx;
	float 		my;
	float 		mz;
}packed_compass_t;

typedef struct
{
	uint64_t 	time_us;
	int16_t		mx;
	int16_t		my;
	int16_t		mz;
}packed_compass_i16_t;

typedef struct
{
	uint64_t 	time_us;
	char 		str[STR_MAX];
}packed_string_t;

typedef struct   
{
	uint64_t 	time_us;
	int32_t 	lon32;		
	int32_t 	lat32;		 
	int32_t 	height;		 
	uint32_t 	hacc;		
	uint32_t 	vacc;	
}packed_gps_posllh_t;

typedef struct   
{
	uint64_t 	time_us;	
	uint8_t 	fix_type_and_status;	
	uint8_t 	snumber;
	uint32_t 	time_to_first_fix;		
}packed_gps_status_t;

typedef struct   
{
	uint64_t 	time_us;	 
	uint8_t 	fix_type_and_status;		
	int32_t 	ecef_x;		
	int32_t 	ecef_y;
	int32_t 	ecef_z;					
	uint32_t 	position_accuracy_3d; 	
	int32_t 	ecef_x_velocity;		
	int32_t 	ecef_y_velocity;
	int32_t 	ecef_z_velocity;	
	uint32_t 	speed_accuracy;
	uint16_t 	position_DOP;
	uint8_t 	satellites;
}packed_gps_solution_t;

typedef struct 
{
	uint64_t 	time_us;
	int32_t 	vel_north;
	int32_t 	vel_east;
	int32_t 	vel_down;
	uint32_t 	speed_3d;
	uint32_t 	speed_2d;			
	int32_t 	heading_2d;			
	uint32_t 	speed_accuracy;
	uint32_t 	heading_accuracy;
}packed_gps_velned_t;

typedef struct
{
	uint64_t 	time_us;
	uint8_t 	state;
	int16_t 	ch[9];
}packed_rc_raw_t;

typedef struct
{
	uint64_t 	time_us;
	float 		q[4];
	float		yaw;
	float		pitch;
	float		roll;
}packed_att_6dof_t;

// debug info
typedef struct
{
	uint64_t 	time_us;
	float		data[3];
}packed_debug_float3_t;

typedef struct
{
	uint64_t 	time_us;
	float		data[4];
}packed_debug_float4_t;

typedef struct
{
	uint64_t 	time_us;
	float		data[6];
}packed_debug_float6_t;

typedef struct
{
	uint64_t 	time_us;
	float		data[8];
}packed_debug_float8_t;

typedef struct
{
	uint64_t 	time_us;
	float		px;
	float		py;
	float		pz;
}packed_gps_pos_ned_t;

typedef struct
{
	uint64_t 	time_us;
	float		vx;
	float		vy;
	float		vz;
}packed_gps_vel_ned_t;

typedef struct
{
	uint64_t 	time_us;
	float		ax;
	float		ay;
	float		az;
}packed_gps_acc_ned_t;

typedef struct
{
	uint64_t 	time_us;
	float		mx;
	float		my;
	float		mz;
	float		yaw;
}packed_compass_measure_t;

typedef struct
{
	uint64_t 	time_us;
	float		altitude;
	float		climbrate; 
	float		climb_filt;
}packed_baro_measure_t;

typedef struct
{
	uint64_t 	time_us;
	float 		px;
	float 		py;
	float 		pz;
	float 		vx;
	float 		vy;
	float 		vz; 
}packed_status_pv_t;

typedef struct
{
	uint64_t 	time_us;
	uint64_t 	timestamp;
}packed_timestamp_t;

#pragma pack()

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif

