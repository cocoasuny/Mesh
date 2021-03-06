#ifndef __IIR_Filter_Parameter_H
#define __IIR_Filter_Parameter_H

#include "cc_alg_types.h"


/* IIR Low Pass, 4.2Hz Cut off, paramters, in format of b10,b11,b12,a11,a12,,,, */ 


#define GSEN_RATIO_BANDSTOP_LEN 	(51)
#define GSEN_RATIO_BANDSTOP_LEN_Half 	(25)

/* 	G sensor IIR Filter 		*/
#define GSEN_IIR_LP_2_3_Stage_Num (5) 

#define GSEN_IIR_HP_0_5_Stage_Num (2) 

#define GSEN_IIR_FILT_BLK_SIZE  (50)

#define GSEN_IIR_HP_0_5_PostShift (1)
#define GSEN_IIR_LP_2_3_PostShift (1)

#define GSEN_IIR_PROC_LEN 		(50)


extern const int16_t GSEN_IIR_LP_2_3_Para[];
/* IIR High Pass, 0.55Hz cut off, paramters, in format of b10,b11,b12,a11,a12,,,, */ 

extern const int16_t GSEN_IIR_HP_0_5_Para[];



#endif /* __IIR_Filter_Parameter_H */
