#include	<stdio.h>
#include	<ctype.h>
#include	<fcntl.h>
#include	<sys/types.h>				/*                                  */ 
#include	<sys/mtio.h>				/* header files for tape operations */ 
#include	<sys/ioctl.h>				/*                                  */ 
#include	<stdlib.h>
#include	<unistd.h>

#include	<buffer_defs.h>
#define		PERMS 0666					/* read/write for owner user and others */

/* Define global variables */    
	int		*messp;						/* Pointer to shared memory message_box */
	int		outfileno;
	int		exades;

int reduc_position( int cup, int bot, int eod, int fileno, int recno, int inout)
{
	int		tapetype_off = 3;
	int		stor1_off    = 2;
	int		stor2_off    = 12;
	int		filerec_off  = 13;
	int		*typep;
	int		*stor1p;
	int		*stor2p;
	int		*filerecp;
	int		current_file;
	int		current_rec;
	int		files2skip;
	int		recs2skip;
	int		blockfac;
	int		type;
	long int	bytepos;
	char		s1[4];
	char		in_file[128];         /* file names */
	char		out_file[128];        /* file names */
	int		n, created;

	struct	mtop 	mt_command;
	struct	mtget mt_status;
	char	msg1[1024] = "Position is - File/Record : ";
	char	msg2[1024] = "Positioning Exabyte to beginning of tape ...";
	char	msg3[1024] = "Positioning Exabyte to end of recorded data ...";
	char	msg4[1024] = "Positioning Exabyte to file : ";
	char	err0d[1024] = "Use the position at file/record no... ";
	char	msg2d[1024] = "Positioning to file : ";
	char	err1d[1024] = "Could not open file in present directory: file_";
	char	err2d[1024] = "Could not position to record : ";
	char	err1[1024]="*** ERROR *** Could not close Exabyte/Disc device file";
	char	errd[1024]="*** ERROR *** Could not open any disc reduc_0-999 in present directory";

	typep    = messp + tapetype_off;
	filerecp = messp + filerec_off;
	stor1p   = messp + stor1_off;
	stor2p   = messp + stor2_off;

	type = 0;
	if (inout == 1) type = *typep / 10;
	if (inout == 2) type = *typep - (*typep / 10) * 10;

	if ( type == 0 ) {
	   blockfac = REC_IN_BUF;			/* SIRIUS format, 4 32k blocks per databuffer */
	}
	if ( type == 1 && inout == 0 ) {
	   blockfac = 64;					/* DAISY input format, 64 1k blocks per databuffer */
	} else {
	   blockfac = REC_IN_BUF;			/* Always SIRIUS output format */
	}
	if ( type == 3 ) {
	   blockfac = REC_IN_BUF;			/* DISK format = SIRIUS format, see above */
	}
	  
	if ( inout == 1 ) {
	   exades = *stor1p;
	}
	if ( inout == 2 ) {
	   exades = *stor2p;
	}

/* Determining current file and record for disk */
	if ( type == 3 && inout == 1) {
		if (exades > 0) {
			current_file = *filerecp / 1000000;
			current_rec  = *filerecp - current_file * 1000000;
		} else {
			current_file = -1;
			current_rec  = 0;
			*filerecp    = 0;
		}
	}
	if ( type == 3 && inout == 2) {
		if (exades > 0) {
/*		  bytepos = tell( exades );*/
		  if(lseek(exades, bytepos, 0) == -1){         
		  				errprint("Could not find current position for output file, recordno set = 0");
				bytepos = 0;
			}

			current_rec  = bytepos / (blockfac * 32 * 1024); 
			current_file = outfileno;
		} else {
			outfileno    = -1;
			current_file =  outfileno;
			current_rec  =  0;
		}
	}

/* -------------------------------------------------- */
	/* CALL OPTION - Current position */
	if ( cup == 1 && type != 3) {
	   mt_command.mt_op = MTNOP;
	   if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	      return -1;
           }  
	   if (ioctl( exades, MTIOCGET, (char *)&mt_status) == -1) {
	      return -1;
           } 

	   wprint( "%s%d/%d\n", msg1, mt_status.mt_fileno, mt_status.mt_blkno/blockfac ); 

      	   return 0;
	}
	if ( cup == 1 && type == 3 ) {
	   wprint( "%s%d/%d\n", msg1, current_file, current_rec ); 

      	   return 0;
	}


	/* -------------------------------------------------- */
	/* CALL OPTION - BOT */
	/* Go to beginning of tape */
	if ( bot == 1 && type != 3) {
	   wprint( "%s\n",msg2 );
           sleep(1);
	   mt_command.mt_op = MTREW;
	   if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	      return -1;
           }          
      	   return 0;
	}

	/* Not in use */
	if ( bot == 1 && type == 3) {
	   errprint( "%s\n",err0d );
      	   return 0;
	}


	/* -------------------------------------------------- */
	/* CALL OPTION - EOD */
	/* Go to end of recorded data */
	if ( eod == 1 && type != 3) {
	   wprint( "%s\n",msg3 );
           sleep(1);
	   mt_command.mt_op = MTEOM;
	   if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	      return -1;
           }          
      	   return 0;
	}

	/* Go to end of recorded data */
	if ( eod == 1 && type == 3) {
	   errprint( "%s\n",err0d );
      	   return 0;
	}


	/* -------------------------------------------------- */
	/* CALL OPTION - POS FILE/REC */
	/* Read the status */

        if ( type != 3) {              /* Case of exabyte-files */
	   mt_command.mt_op = MTNOP;
	   if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	      return -1;
           }  
	   if (ioctl( exades, MTIOCGET, (char *)&mt_status) == -1) {
	      return -1;
           } 

	   current_file = mt_status.mt_fileno;
	   current_rec  = mt_status.mt_blkno;           
           if (fileno  == -1) fileno = current_file;
	   files2skip   = fileno - current_file;
	   recs2skip    = (recno * blockfac) - current_rec;


	   /* Case 1: Move to File x, Record 0 */
	   if ( files2skip != 0 && recno == 0 ) {
	      wprint( "%s%d\n",msg4, fileno );
              sleep(1);
	      mt_command.mt_op = MTFSF;
	      mt_command.mt_count = files2skip;
	      if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	         return -1;
              }           
      	      return 0;
	   }



		/* Case 2: Move first to File x Rec 0, Then to Record y */
		/* Not Used in current version !                        */
		if ( files2skip != 0 && recno != 0 ) {

	      mt_command.mt_op = MTFSF;
	      mt_command.mt_count = files2skip;
	      if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	         return -1;
              }           
	      mt_command.mt_op = MTFSR;
	      mt_command.mt_count = recno * blockfac;
	      if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	         return -1;
              }    
	      return 0;       
	   }

	
	   /* Case 3: Move to Record x in current file */
	   /* OK for offline sort !                    */
	   if ( files2skip == 0 && recs2skip != 0 ) {
	      mt_command.mt_op = MTFSR;
	      mt_command.mt_count = recs2skip;
	      if (ioctl( exades, MTIOCTOP, &mt_command) == -1) {
	         return -1;
              }           
      	      return 0;
	   }


	   /* Case 4: Don't do anything at all ... */
	   if ( files2skip == 0 && recs2skip == 0 ) {
	      wprint( "%s%d %d\n", msg1, mt_status.mt_fileno, mt_status.mt_blkno/blockfac );
	   }
	}   
	
	if ( type == 3) {										/* Case of disk-files */
		if (fileno == -1) fileno = current_file;
	   files2skip   = fileno - current_file;
	   recs2skip    = recno  - current_rec;
		if ( files2skip == 0 && recs2skip == 0 ){
	      wprint( "%s%d %d\n", msg1, fileno, recno );
		} else {
		if(inout == 1) {
			number2string(fileno, s1);						/* Move to fileno and recno */
			sprintf(in_file,"file_%s",s1);
			if ((exades = open(in_file,O_RDONLY)) == -1) {	/* file do not exist */
				errprint("%s%d\n",err1d,fileno);
				return -1;
			} else {
				bytepos = (blockfac*32*1024) * recno;
	            if(lseek(exades, bytepos, 0) == -1){        /* record do not exist */
						errprint("%s %d\n",err2d,recno);
						exades=0;
						return -1;
					}  else {
	               wprint( "%s%d/%d\n", msg1, fileno, recno ); 
						*filerecp  = fileno * 1000000 + recno;
						close(*stor1p);
						*stor1p    = exades;
						return 0;
					}
				}
			}

			if (inout == 2) {
				number2string(fileno, s1);					/* Move to fileno and recno */
				sprintf(out_file,"reduc_%s",s1);
				if ((exades = open(out_file,O_RDWR)) != -1) { /* file exist */
					errprint("Warning, you are writing to existing file: %s\n",out_file);
					type      = *typep / 10;
					*typep    = type * 10 + 3;				/* disk is output device  */
				} else {
					if ( *stor2p > 0){
						wprint("Closing files/rewinding...\n");
						sleep(1);
						if ( close( *stor2p ) == -1) {
							errprint("%s\n",err1);
						}
					}
					created = -1;
					for (n = 0; n < 1000; n++){				/*Finding last filenumber used*/
						number2string(n, s1);
						sprintf(out_file,"reduc_%s",s1);
						if ((exades = open(out_file,O_RDWR)) == -1) { /* file non-exists */
							created  = 1;
							break;
						}
					}
					
					if ( created == -1){
						errprint("%s\n",errd);
						exades  = 0;
						type    = *typep / 10;
						*typep  = type * 10 + 0;
					} else {								/* create a new file reduc_0 */
						if ((exades = creat(out_file,PERMS)) == -1) {
							wprint("Could not create outfile %s\n",out_file);
						} 
						exades = open(out_file,O_RDWR);
						type      = *typep / 10;
						*typep    = type * 10 + 3;           /* disk is output device  */
						outfileno = n;
						recno     = 0;
					} 
				}
				bytepos = (blockfac*32*1024) * recno;
	         if(lseek(exades,bytepos,0) == -1){				/* record do not exist */
					errprint("%s %d\n",err2d,recno);
					exades = 0;
					return -1;
				}  else {
	            wprint( "%s%d/%d\n", msg1, fileno, recno ); 
					outfileno = fileno;
					close(*stor2p);
					*stor2p   = exades;
					return 0;
				}
			}
		}
	}
}