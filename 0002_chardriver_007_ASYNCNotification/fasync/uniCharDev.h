#ifndef _UNI_CHAR_DEV_H_
#define _UNI_CHAR_DEV_H_

#include <asm/ioctl.h>

#define DEV_NAME "uniCharDevFasync" //This will be shown in cat /proc/devices
#define DEV_NODE "/dev/uniCharDevFasync"

/* ioctl constants are defined in this header file for
   uniCharDevOpsIOCtl driver */

/* We made this header file because the application can include this and will
use the request commands*/

/* Constants have to be globally unique. Though this is not
   compulsory, it is necessary. This is because of the simple
   reason: we do not want our commands to clash with the commands
   in other drivers. we dont want the right command going to
   the wrong driver or vice-versa */

/* We have to base our constants based on a magic number */
#define UNI_MAGIC 'U'

/* Defining constants require us to decide folllowing values

	1. type or magic number (type)
	2. sequence number which is eigth bits wide. This means
	   we can have up to 256 ioctl commands (nr)
	3. direction if the arg is involved, whether we are reading or writing
	4. size(composition) of the arguement.

   To arrive at unique numbers easily we use the following macros
   _IO(type, nr);
   _IOW(type, nr, dataitem) the fourth item calculated using sizeof
   _IOR(type, nr, dataitem)
   _IOWR(type, nr, dataitem)
*/

/* UNI_FILL_ZERO: to fill our character array with zeros
   As we can see there is not data transfer. The moment
   our driver gets this command, it executes the following
   for loop
   	for(i=0; i<MAX_LENGTH; i++) char_device_buf[i] = 0;
*/
#define UNI_FILL_ZERO		_IO(UNI_MAGIC, 0)

/* UNI_FILL_ZERO: to fill our character array with a character
   As we can see there is not data transfer. The moment
   our driver gets this command, it executes the following
   for loop
   	for(i=0; i<MAX_LENGTH; i++) char_device_buf[i] = c;
   where c is the character. This involves something to be
   transfered from user to kernel
*/
#define UNI_FILL_CHAR		_IOW(UNI_MAGIC, 1, char)

/* UNI_SET_SIZE: to set the size of the character array */

#define UNI_SET_SIZE		_IOW(UNI_MAGIC, 2, uint)

/* UNI_GET_SIZE: to get the size of the character array */
#define UNI_GET_SIZE		_IOR(UNI_MAGIC, 3, uint)

#define UNI_MAX_CMDS		4

#endif /* _UNI_CHAR_DEVICE_H_ */
