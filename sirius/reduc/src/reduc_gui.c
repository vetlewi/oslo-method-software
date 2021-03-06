#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<stdlib.h>

#include	<Xm/RowColumn.h>
#include	<Xm/CascadeB.h>
#include	<Xm/SeparatoG.h>
#include	<Xm/PushBG.h>
#include	<Xm/PushB.h>
#include	<Xm/MainW.h>
#include	<Xm/Text.h>
#include	<Xm/LabelG.h>
#include	<Xm/CascadeBG.h>
#include	<Xm/ToggleB.h>
#include	<Xm/ToggleBG.h>
#include	<Xm/SelectioB.h>
#include	<Xm/FileSB.h>
#include	<Xm/DrawnB.h>
#include	<Xm/MessageB.h>

/* Define global variables */    
int			*messp;			/* Pointer to shared memory message_box */
int			outfileno;

int			Is_Started		= 0;	/* Status variable, 0 = NOT Started, 1 = Started */
int			Media1_On		= 0;	/* Status variable, 0 = NO Media, 1 = Media On */
int			Media2_On		= 0;	/* Status variable, 0 = NO Media, 1 = Media On */
int			Input_drive_no  = 0;	/* Drive number for data input exabyte */
int			Output_drive_no = 0;	/* Drive number for data input exabyte */

Widget 		toplevel, rowcol, newcol, MainWindow;
Widget 		text_w, error_w;	/* Message output and error output windows */
Widget		quitm, testm;
Widget		filem, file_quit, file_options;
Widget 		runm, run_sort, sort_whole, sort_file, sort_recs, run_reduct, reduct_whole, reduct_file, reduct_recs;
Widget		run_stop, run_status, ExtractPullRight, ReductPullRight;
Widget		tape1m, tape1_device, tape1_unload, Device1PullRight, Position1PullRight;
Widget		tape1_position, position1_current, position1_bot, position1_eod, position1_file, position1_record;
Widget		device1_none, device1_exb1, device1_exb2, device1_disc;
Widget		tape2m, tape2_device, tape2_unload, Device2PullRight, Position2PullRight;
Widget		tape2_position, position2_current, position2_bot, position2_eod, position2_file;
Widget		device2_none, device2_exb1, device2_exb2, device2_disc;
Widget		shiftm, shift_init, shift_file, shift_show, shift_adjust;
Widget		peakm, peak_init, peak_file, peak_show, result_show;
Widget		winm, win_file, win_show;
Widget 		helpm, help_about, help_install, help_reduc;
Widget		updateb, peakb;
Pixmap		pixmap,pixmap1,pixmap2;

void		filecopy(FILE *,FILE *); 

main(argc, argv)
int argc;
char *argv[];
{    

    Widget		MenuBar; 
    Widget		FilePullDown, RunPullDown, Media1PullDown, Media2PullDown;
    Widget		ShiftPullDown, PeakPullDown, WinPullDown, HelpPullDown;
    XmString    	file, run, tape1, tape2, shift, peak, win, help;
    XmString		quit, quit_accel_text, text;
    XtAppContext 	app;
    void		quit_pushed();
    void		options_pushed();
    void		sortwhole_pushed();
    void		sortfile_pushed();
    void		sortrecs_pushed();
    void		reductwhole_pushed();
    void		reductfile_pushed();
    void		reductrecs_pushed();
    void		stop_pushed();
    void		status_pushed();
    void		none_device1_pushed();		
    void		exb1_device1_pushed();		
    void		exb2_device1_pushed();
    void		disc_device1_pushed();
    void		current1_position_pushed();		
    void		bot1_position_pushed();		
    void		eod1_position_pushed();
    void		file1_position_pushed();	
    void		record1_position_pushed();	
    void		unload1_pushed();
    void		none_device2_pushed();		
    void		exb1_device2_pushed();		
    void		exb2_device2_pushed();
    void		disc_device2_pushed();	
    void		current2_position_pushed();		
    void		bot2_position_pushed();		
    void		eod2_position_pushed();
    void		file2_position_pushed();	
    void		record2_position_pushed();	
    void		unload2_pushed();
    void		shiftinit_pushed();
    void		shiftfile_pushed();
    void		shiftshow_pushed();
    void		shiftadjust_pushed();
    void		peakinit_pushed();
    void		peakfile_pushed();
    void		peakshow_pushed();
    void		resultshow_pushed();
    void		wininit_pushed();	
    void		winfile_pushed();
    void		winshow_pushed();	
    void		h_about_pushed();
	void		h_install_pushed();
	void		h_reduc_pushed();
    void		info_pushed();
    void		peak_pushed();

    Pixel		fg, bg;
    Dimension	ht, st;

    Arg			args[7];
    int			n = 0;
    int			status;				     
    char		*cc_quit = "Ctrl<Key>Q";

    /* ------------------------------------------------------------------ */
    /* ------------------------------------------------------------------ */
    /* Initialize the data reduction system shared memory                 */
    if ( reduc_init( ) == -1) {
       printf("\n **** ERROR **** Initialization failed - exiting \n");
       exit(0);
    } 
    shiftinit_pushed();

    /* ------------------------------------------------------------------ */
    /* ------------------------------------------------------------------ */
    /* Build the GUI, start with Motif top level routines		  */
    XtSetLanguageProc (NULL, NULL, NULL);
    toplevel = XtVaAppInitialize (&app, "Reduc", NULL, 0, &argc, argv, NULL, 
					XmNminWidth,	500,
					XmNminHeight,	600,
					XmNmaxWidth,	600,
					XmNmaxHeight,	650,				  
					NULL);

    MainWindow = XtVaCreateManagedWidget("main_w",
       					xmMainWindowWidgetClass,toplevel,
        				XmNscrollingPolicy,  	XmAUTOMATIC,
        				NULL);


    /* Define a RowColumn manager widget as child of MainWindow */
    rowcol = XtVaCreateManagedWidget 	("rowcolumn", 
					xmRowColumnWidgetClass, MainWindow,
					XmNnumColumns, 		3,
                                        XmNorientation, 	XmVERTICAL,
					XmNisAligned,		True,
                                      	NULL);


  /* Define another RowColumn manager widget as child of rowcol */
    newcol = XtVaCreateManagedWidget 	("newcolumn", 
					xmRowColumnWidgetClass, rowcol,
                                      	NULL);


    /* ------------------------------------------------------------------ */
    /* ------------------------------------------------------------------ */
    /* Create the menubar                                                 */
    MenuBar = XmCreateMenuBar 		(MainWindow, 
					"MenuBar", 
					NULL, 0); 



    /* ------------------------------------------------------------------ */
    /* create the "File" pulldown menu                                    */
    FilePullDown = XmCreatePulldownMenu (MenuBar, 
					"FilePullDown", 
					NULL, 0);
    file = XmStringCreateLocalized 	("File");
    filem = XtVaCreateManagedWidget 	("File", 
             				xmCascadeButtonWidgetClass, MenuBar,
             				XmNlabelString,  	file,
             				XmNmnemonic,    	'F',
             				XmNsubMenuId,    	FilePullDown,
             				NULL);
    XmStringFree 			( file ); 



    /* Add the menu item OPTIONS */
    file_options = XtVaCreateManagedWidget ("Options",
                                      	xmPushButtonGadgetClass, 
                                      	FilePullDown, 
                                        NULL);
    XtAddCallback (file_options, XmNactivateCallback, options_pushed, NULL);  


    XtVaCreateManagedWidget ("separator", xmSeparatorGadgetClass, FilePullDown, NULL);

    /* Add the menu item QUIT */
    quit_accel_text = XmStringCreateLocalized("Ctrl+Q");
    file_quit = XtVaCreateManagedWidget ("Quit",
                                      	xmPushButtonGadgetClass, 
                                      	FilePullDown, 
             			      	XmNmnemonic,    	'Q',
				      	XmNacceleratorText, 	quit_accel_text,
				     	XmNaccelerator,	    	cc_quit,
                                      NULL);
    XtAddCallback (file_quit, XmNactivateCallback, quit_pushed, NULL);  


    /* ------------------------------------------------------------------ */
    /* create the "Run" pulldown menu */
    RunPullDown = XmCreatePulldownMenu (MenuBar, "RunPullDown", NULL, 0);
    run = XmStringCreateLocalized ("Run");
    runm = XtVaCreateManagedWidget ("Run", 
             xmCascadeButtonWidgetClass, MenuBar,
             XmNlabelString,  run,
             XmNmnemonic,    'R',
             XmNsubMenuId,    RunPullDown,
             NULL);
    XmStringFree ( run ); 



    /* Add the menu item "Shift Extraction" */
    ExtractPullRight = XmCreatePulldownMenu( RunPullDown, "ExtractPullRight", NULL, 0);

    run_sort = XtVaCreateManagedWidget ("Shift extraction",
                                      xmCascadeButtonGadgetClass, RunPullDown,
                                      XmNsubMenuId, ExtractPullRight, 
                                      NULL);

    sort_whole = XtVaCreateManagedWidget ("Whole Tape/Disk",
                                      xmPushButtonGadgetClass,
                                      ExtractPullRight, 
                                      NULL);
    sort_file = XtVaCreateManagedWidget ("Files ...",
                                      xmPushButtonGadgetClass,
                                      ExtractPullRight, 
                                      NULL);
    sort_recs = XtVaCreateManagedWidget ("Records ...",
                                      xmPushButtonGadgetClass,
                                      ExtractPullRight, 
                                      NULL);

    XtAddCallback (sort_whole, XmNactivateCallback,  sortwhole_pushed, NULL);
    XtAddCallback (sort_file,  XmNactivateCallback,  sortfile_pushed, NULL); 
    XtAddCallback (sort_recs,  XmNactivateCallback,  sortrecs_pushed, NULL); 

    /* Add the menu item "Data reduction" */
    ReductPullRight = XmCreatePulldownMenu( RunPullDown, "ReductPullRight", NULL, 0);

    run_reduct = XtVaCreateManagedWidget ("Data reduction",
                                      xmCascadeButtonGadgetClass, RunPullDown,
                                      XmNsubMenuId, ReductPullRight, 
                                      NULL);

    reduct_whole = XtVaCreateManagedWidget ("Whole Tape/Disk",
                                      xmPushButtonGadgetClass,
                                      ReductPullRight, 
                                      NULL);
    reduct_file = XtVaCreateManagedWidget ("Files ...",
                                      xmPushButtonGadgetClass,
                                      ReductPullRight, 
                                      NULL);
    reduct_recs = XtVaCreateManagedWidget ("Records ...",
                                      xmPushButtonGadgetClass,
                                      ReductPullRight, 
                                      NULL);
 
    XtAddCallback (reduct_whole, XmNactivateCallback,  reductwhole_pushed, NULL);
    XtAddCallback (reduct_file,  XmNactivateCallback,  reductfile_pushed, NULL); 
    XtAddCallback (reduct_recs,  XmNactivateCallback,  reductrecs_pushed, NULL); 



    /* Add the menu item STOP */
    run_stop = XtVaCreateManagedWidget ("Stop",
                                      xmPushButtonGadgetClass, 
                                      RunPullDown, 
                                      NULL);
    XtAddCallback (run_stop, XmNactivateCallback, stop_pushed, NULL); 

    XtVaCreateManagedWidget ("separator",
        			      xmSeparatorGadgetClass, RunPullDown, NULL);

    /* Add the menu item INFO */
    run_status = XtVaCreateManagedWidget ("Info",
                                      xmPushButtonGadgetClass, 
                                      RunPullDown, 
                                      NULL);
    XtAddCallback (run_status, XmNactivateCallback, status_pushed, NULL); 


    /* ------------------------------------------------------------------ */
    /* create the "Input device" pulldown menu */
    Media1PullDown = XmCreatePulldownMenu (MenuBar, "Media1PullDown", NULL, 0);
    tape1 = XmStringCreateLocalized ("Input");
    tape1m = XtVaCreateManagedWidget ("Input device", 
             xmCascadeButtonWidgetClass, MenuBar,
             XmNlabelString,  tape1,
             XmNmnemonic,    'I',
             XmNsubMenuId,    Media1PullDown,
             NULL);
    XmStringFree ( tape1 ); 

    /* Add the menu DEVICE */
    Device1PullRight = XmCreatePulldownMenu( Media1PullDown, "Device1PullRight", NULL, 0);

    tape1_device = XtVaCreateManagedWidget ("Device",
                                      xmCascadeButtonGadgetClass, Media1PullDown,
                                      XmNsubMenuId, Device1PullRight, 
                                      NULL);

    device1_none = XtVaCreateManagedWidget ("None",
                                      xmPushButtonGadgetClass,
                                      Device1PullRight, 
                                      NULL);
    device1_exb1 = XtVaCreateManagedWidget ("Exabyte 1",
                                      xmPushButtonGadgetClass,
                                      Device1PullRight, 
                                      NULL);
    device1_exb2 = XtVaCreateManagedWidget ("Exabyte 2",
                                      xmPushButtonGadgetClass,
                                      Device1PullRight, 
                                      NULL);
    device1_disc = XtVaCreateManagedWidget ("Disk",
                                      xmPushButtonGadgetClass,
                                      Device1PullRight, 
                                      NULL);
    XtAddCallback (device1_none, XmNactivateCallback, none_device1_pushed, NULL);
    XtAddCallback (device1_exb1, XmNactivateCallback, exb1_device1_pushed, NULL);
    XtAddCallback (device1_exb2, XmNactivateCallback, exb2_device1_pushed, NULL);
    XtAddCallback (device1_disc, XmNactivateCallback, disc_device1_pushed, NULL);



    /* Add the menu POSTITION */
    Position1PullRight = XmCreatePulldownMenu( Media1PullDown, "Position1PullRight", NULL, 0);

    tape1_position = XtVaCreateManagedWidget ("Position at",
                                      xmCascadeButtonGadgetClass, Media1PullDown,
                                      XmNsubMenuId, Position1PullRight, 
                                      NULL);

    position1_current = XtVaCreateManagedWidget ("Current position",
                                      xmPushButtonGadgetClass,
                                      Position1PullRight, 
                                      NULL);
    position1_bot = XtVaCreateManagedWidget ("Beginning of Tape/Disk",
                                      xmPushButtonGadgetClass,
                                      Position1PullRight, 
                                      NULL);
    position1_file= XtVaCreateManagedWidget ("File no...",
                                      xmPushButtonGadgetClass,
                                      Position1PullRight, 
                                      NULL);
    position1_record= XtVaCreateManagedWidget ("Record no...",
                                      xmPushButtonGadgetClass,
                                      Position1PullRight, 
                                      NULL);

 
    XtAddCallback (position1_current, XmNactivateCallback, current1_position_pushed, NULL);
    XtAddCallback (position1_bot, XmNactivateCallback, bot1_position_pushed, NULL);
    XtAddCallback (position1_file, XmNactivateCallback, file1_position_pushed, NULL); 
    XtAddCallback (position1_record, XmNactivateCallback, record1_position_pushed, NULL); 


    XtVaCreateManagedWidget ("separator",
        			      xmSeparatorGadgetClass, Media1PullDown, NULL);

    /* Add the menu item UNLOAD */
    tape1_unload = XtVaCreateManagedWidget ("Unload/close",
                                      xmPushButtonGadgetClass, 
                                      Media1PullDown, 
                                      NULL);
    XtAddCallback (tape1_unload, XmNactivateCallback, unload1_pushed, NULL); 


    /* ------------------------------------------------------------------ */
    /* create the "Output device" pulldown menu */
    Media2PullDown = XmCreatePulldownMenu (MenuBar, "Media2PullDown", NULL, 0);
    tape2 = XmStringCreateLocalized ("Output");
    tape2m = XtVaCreateManagedWidget ("Output device", 
             xmCascadeButtonWidgetClass, MenuBar,
             XmNlabelString,  tape2,
             XmNmnemonic,    'O',
             XmNsubMenuId,    Media2PullDown,
             NULL);
    XmStringFree ( tape2 ); 



    /* Add the menu DEVICE */
    Device2PullRight = XmCreatePulldownMenu( Media2PullDown, "Device2PullRight", NULL, 0);

    tape2_device = XtVaCreateManagedWidget ("Device",
                                      xmCascadeButtonGadgetClass, Media2PullDown,
                                      XmNsubMenuId, Device2PullRight, 
                                      NULL);

    device2_none = XtVaCreateManagedWidget ("None",
                                      xmPushButtonGadgetClass,
                                      Device2PullRight, 
                                      NULL);
    device2_exb1 = XtVaCreateManagedWidget ("Exabyte 1",
                                      xmPushButtonGadgetClass,
                                      Device2PullRight, 
                                      NULL);
    device2_exb2 = XtVaCreateManagedWidget ("Exabyte 2",
                                      xmPushButtonGadgetClass,
                                      Device2PullRight, 
                                      NULL);
    device2_disc = XtVaCreateManagedWidget ("Disk",
                                      xmPushButtonGadgetClass,
                                      Device2PullRight, 
                                      NULL);
    XtAddCallback (device2_none, XmNactivateCallback, none_device2_pushed, NULL);
    XtAddCallback (device2_exb1, XmNactivateCallback, exb1_device2_pushed, NULL);
    XtAddCallback (device2_exb2, XmNactivateCallback, exb2_device2_pushed, NULL);
    XtAddCallback (device2_disc, XmNactivateCallback, disc_device2_pushed, NULL);


    /* Add the menu POSTITION */
    Position2PullRight = XmCreatePulldownMenu( Media2PullDown, "Position2PullRight", NULL, 0);

    tape2_position = XtVaCreateManagedWidget ("Position at",
                                      xmCascadeButtonGadgetClass, Media2PullDown,
                                      XmNsubMenuId, Position2PullRight, 
                                      NULL);

    position2_current = XtVaCreateManagedWidget ("Current position",
                                      xmPushButtonGadgetClass,
                                      Position2PullRight, 
                                      NULL);
    position2_bot = XtVaCreateManagedWidget ("Beginning of Tape/Disk",
                                      xmPushButtonGadgetClass,
                                      Position2PullRight, 
                                      NULL);
    position2_eod = XtVaCreateManagedWidget ("End of Data",
                                      xmPushButtonGadgetClass,
                                      Position2PullRight, 
                                      NULL);
    position2_file= XtVaCreateManagedWidget ("File no...",
                                      xmPushButtonGadgetClass,
                                      Position2PullRight, 
                                      NULL);

 
    XtAddCallback (position2_current, XmNactivateCallback, current2_position_pushed, NULL);
    XtAddCallback (position2_bot, XmNactivateCallback, bot2_position_pushed, NULL);
    XtAddCallback (position2_eod, XmNactivateCallback, eod2_position_pushed, NULL);
    XtAddCallback (position2_file, XmNactivateCallback, file2_position_pushed, NULL); 


    XtVaCreateManagedWidget ("separator",
        			      xmSeparatorGadgetClass, Media2PullDown, NULL);


   /* Add the menu item UNLOAD */
    tape2_unload = XtVaCreateManagedWidget ("Unload/close",
                                      xmPushButtonGadgetClass, 
                                      Media2PullDown, 
                                      NULL);
    XtAddCallback (tape2_unload, XmNactivateCallback, unload2_pushed, NULL); 



    /* ------------------------------------------------------------------ */
    /* create the "Shift" pulldown menu */
    ShiftPullDown = XmCreatePulldownMenu (MenuBar, "ShiftPullDown", NULL, 0);
    shift  = XmStringCreateLocalized ("Shift");
    shiftm = XtVaCreateManagedWidget ("Shift", 
             xmCascadeButtonWidgetClass, MenuBar,
             XmNlabelString,  shift,
             XmNmnemonic,    'S',
             XmNsubMenuId,    ShiftPullDown,
             NULL);
    XmStringFree ( shift ); 

    /* Add the menu item INITIALIZE shift factors */
    shift_init = XtVaCreateManagedWidget ("Initialize",
                                      xmPushButtonGadgetClass, 
                                      ShiftPullDown, 
                                      NULL);
    XtAddCallback (shift_init, XmNactivateCallback, shiftinit_pushed, NULL);


    /* Add the menu item From File ... sorting application */
    shift_file = XtVaCreateManagedWidget ("From File ...",
                                      xmPushButtonGadgetClass, 
                                      ShiftPullDown, 
                                      NULL);
    XtAddCallback (shift_file, XmNactivateCallback, shiftfile_pushed, NULL);


    /* Add the menu item Show values sorting application */
    XtVaCreateManagedWidget ("separator", xmSeparatorGadgetClass, ShiftPullDown, NULL);
    shift_show = XtVaCreateManagedWidget ("Show values",
                                      xmPushButtonGadgetClass, 
                                      ShiftPullDown, 
                                      NULL);
    XtAddCallback (shift_show, XmNactivateCallback, shiftshow_pushed, NULL);

   /* Add the menu item adjust shift for every n.th record */
    XtVaCreateManagedWidget ("separator", xmSeparatorGadgetClass, ShiftPullDown, NULL);
    shift_adjust = XtVaCreateManagedWidget ("Shift adjustment",
                                      xmPushButtonGadgetClass, 
                                      ShiftPullDown, 
                                      NULL);
    XtAddCallback (shift_adjust, XmNactivateCallback, shiftadjust_pushed, NULL);



    /* ------------------------------------------------------------------ */
    /* create the "Peak" pulldown menu */


    PeakPullDown = XmCreatePulldownMenu (MenuBar, "PeakPullDown", NULL, 0);
    peak  = XmStringCreateLocalized ("Peak");
    peakm = XtVaCreateManagedWidget ("Peak", 
             xmCascadeButtonWidgetClass, MenuBar,
             XmNlabelString,  peak,
             XmNmnemonic,    'P',
             XmNsubMenuId,    PeakPullDown,
             NULL);
    XmStringFree ( peak ); 

    /* Add the menu item INITIALIZE peak target values */
    peak_init = XtVaCreateManagedWidget ("Initialize",
                                      xmPushButtonGadgetClass, 
                                      PeakPullDown, 
                                      NULL);
    XtAddCallback (peak_init, XmNactivateCallback, peakinit_pushed, NULL);

    peak_file = XtVaCreateManagedWidget ("From File ...",
                                      xmPushButtonGadgetClass, 
                                      PeakPullDown, 
                                      NULL);
    XtAddCallback (peak_file, XmNactivateCallback, peakfile_pushed, NULL);


    XtVaCreateManagedWidget ("separator", xmSeparatorGadgetClass, PeakPullDown, NULL);
    peak_show = XtVaCreateManagedWidget ("Target values",
                                      xmPushButtonGadgetClass, 
                                      PeakPullDown, 
                                      NULL);
    XtAddCallback (peak_show, XmNactivateCallback, peakshow_pushed, NULL);


    XtVaCreateManagedWidget ("separator", xmSeparatorGadgetClass, PeakPullDown, NULL);
    result_show = XtVaCreateManagedWidget ("Results",
                                      xmPushButtonGadgetClass, 
                                      PeakPullDown, 
                                      NULL);
    XtAddCallback (result_show, XmNactivateCallback, resultshow_pushed, NULL);

  
    /* ------------------------------------------------------------------ */
    /* create the "Window" pulldown menu */


    WinPullDown = XmCreatePulldownMenu (MenuBar, "WinPullDown", NULL, 0);
    win  = XmStringCreateLocalized ("Window");
    winm = XtVaCreateManagedWidget ("Window", 
             xmCascadeButtonWidgetClass, MenuBar,
             XmNlabelString,  win,
             XmNmnemonic,    'W',
             XmNsubMenuId,    WinPullDown,
             NULL);
    XmStringFree ( win ); 

    win_file = XtVaCreateManagedWidget ("From File ...",
                                      xmPushButtonGadgetClass, 
                                      WinPullDown, 
                                      NULL);
    XtAddCallback (win_file, XmNactivateCallback, winfile_pushed, NULL);


    XtVaCreateManagedWidget ("separator", xmSeparatorGadgetClass, WinPullDown, NULL);
    win_show = XtVaCreateManagedWidget ("Show values",
                                      xmPushButtonGadgetClass, 
                                      WinPullDown, 
                                      NULL);
    XtAddCallback (win_show, XmNactivateCallback, winshow_pushed, NULL);

    /* ------------------------------------------------------------------ */
    /* create the "Help" pulldown menu */
    HelpPullDown = XmCreatePulldownMenu (MenuBar, "HelpPullDown", NULL, 0);

    /* create the "Help" button ) */
    help = XmStringCreateLocalized ("Help");
    helpm = XtVaCreateManagedWidget ("Help", 
             xmCascadeButtonWidgetClass, MenuBar,
             XmNlabelString,  help,
             XmNmnemonic,    'H',
             XmNsubMenuId,    HelpPullDown,
             NULL);
    XmStringFree ( help ); 

    /* Postition HELP to right */
    XtVaSetValues (MenuBar, XmNmenuHelpWidget, helpm, NULL);

    /* Add menu "About" */
    help_about = XtVaCreateManagedWidget 	("About Reduc",
              					xmPushButtonGadgetClass, 
						HelpPullDown, 
						NULL);
    XtAddCallback (help_about, XmNactivateCallback, h_about_pushed, NULL);

    /* Add menu "Install" */
    help_install = XtVaCreateManagedWidget 	("Install",
        					xmPushButtonGadgetClass, 
						HelpPullDown, 
						NULL);
    XtAddCallback (help_install, XmNactivateCallback, h_install_pushed, NULL);

    /* Add menu "Reduc" */
    help_reduc = XtVaCreateManagedWidget 	("Reduc",
        					xmPushButtonGadgetClass, 
						HelpPullDown, 
						NULL);
    XtAddCallback (help_reduc, XmNactivateCallback, h_reduc_pushed, NULL);

    XtManageChild (MenuBar);




    /* ------------------------------------------------------------------ */    
    /* Add pushbuttons below the menubar */

    XtVaSetValues (	newcol, 
			XmNpacking, 	XmPACK_COLUMN,
			XmNnumColumns, 	1,
			XmNorientation, XmHORIZONTAL,
			NULL); 

    XtVaGetValues (	newcol,
        		XmNforeground, &fg,
        		XmNbackground, &bg,
        		NULL);

    pixmap1 = XmGetPixmap (XtScreen (newcol), "/Applications/sirius/bitmap/info", fg, bg);
    updateb = XtVaCreateManagedWidget ("updateb", 
					xmDrawnButtonWidgetClass, newcol, 
					NULL);
    XtVaGetValues (	updateb, 
        		XmNhighlightThickness, &ht,
        		XmNshadowThickness, &st,
        		NULL);

    XtVaSetValues( updateb, XmNwidth,  2 * ht +2 * st + 26,
                            XmNheight, 2 * ht +2 * st + 26,
                            NULL);
    XtAddCallback (updateb, XmNactivateCallback, info_pushed, NULL);
    XtAddCallback (updateb, XmNexposeCallback, info_pushed, NULL);




    XtVaGetValues (	newcol,
        		XmNforeground, &fg,
        		XmNbackground, &bg,
        		NULL);

    pixmap2 = XmGetPixmap (XtScreen (newcol), "/Applications/sirius/bitmap/peak", fg, bg);
    peakb = XtVaCreateManagedWidget ("peakb", 
					xmDrawnButtonWidgetClass, newcol, 
					NULL);
    XtVaGetValues (	peakb, 
        		XmNhighlightThickness, &ht,
        		XmNshadowThickness, &st,
        		NULL);

    XtVaSetValues( peakb, XmNwidth,  2 * ht +2 * st + 26,
                            XmNheight, 2 * ht +2 * st + 26,
                            NULL);
    XtAddCallback (peakb, XmNactivateCallback, peak_pushed, NULL);
    XtAddCallback (peakb, XmNexposeCallback, peak_pushed, NULL);



    XtManageChild (newcol);
   



    /* ------------------------------------------------------------------ */

    /* Create a text widget for program output */
    XtVaCreateManagedWidget ( "Messages:", 		xmLabelGadgetClass, rowcol, NULL);
    XtSetArg ( args[n], XmNrows,			20); n++;
    XtSetArg ( args[n], XmNcolumns,			50); n++;
    XtSetArg ( args[n], XmNeditable,			FALSE); n++;
    XtSetArg ( args[n], XmNeditMode,			XmMULTI_LINE_EDIT); n++;
    XtSetArg ( args[n], XmNscrollHorizontal, 		FALSE); n++;
    XtSetArg ( args[n], XmNwordWrap, 			TRUE); n++;

    text_w = XmCreateScrolledText ( rowcol, "text_w", args, n );
    XtVaSetValues(text_w, XtVaTypedArg,XmNforeground, XmRString,"MidnightBlue",13,NULL);
    XtManageChild (text_w); 

    /* Create a text widget for error output */
    n=0;
    XtVaCreateManagedWidget ( "Errors:", 		xmLabelGadgetClass, rowcol, NULL);
    XtSetArg ( args[n], XmNrows,			5); n++;
    XtSetArg ( args[n], XmNcolumns,			50); n++;
    XtSetArg ( args[n], XmNeditable,			FALSE); n++;
    XtSetArg ( args[n], XmNeditMode,			XmMULTI_LINE_EDIT); n++;
    XtSetArg ( args[n], XmNscrollHorizontal, 		FALSE); n++;
    XtSetArg ( args[n], XmNwordWrap, 			TRUE); n++;

    error_w = XmCreateScrolledText ( rowcol, "error_w" ,args, n);
    XtVaSetValues(error_w, XtVaTypedArg,XmNforeground, XmRString,"red",7,NULL);
    XtManageChild (error_w); 

    XtManageChild (rowcol);   

    status = set_pushbutton_status();


    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}




/* ============================================================================*/
/*               This section contains the call-back routines                  */
/* ============================================================================*/


/* --------------------------------------------------------------------------- */
void quit_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	Widget 		ex_dialog;
	XmString	yes, no, msg;	
	void	exit_prog();

	ex_dialog = XmCreateQuestionDialog (MainWindow, "REDUC: quit confirmation", NULL, 0);
	yes = XmStringCreateLocalized("Yes");
	no  = XmStringCreateLocalized("No");
	msg = XmStringCreateLocalized("Are you sure you want to quit REDUC?");
	XtVaSetValues(	ex_dialog,
			XmNmessageString,	msg,
			XmNokLabelString,	yes,
			XmNcancelLabelString,	no,
			NULL);
	

	XtAddCallback (ex_dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
	XtAddCallback (ex_dialog, XmNokCallback, exit_prog, widget);
 	XmStringFree (yes);
	XmStringFree (no);
	XmStringFree (msg);


   	XtSetSensitive ( XmMessageBoxGetChild (ex_dialog, XmDIALOG_HELP_BUTTON), False);
	XtUnmanageChild( XmMessageBoxGetChild (ex_dialog, XmDIALOG_HELP_BUTTON));
    	XtManageChild (ex_dialog);

   	XtPopup (XtParent (ex_dialog), XtGrabNone);

 
}
void exit_prog(Widget widget, XtPointer client_data, XtPointer call_data)
{

	char	err2[1024] = "*** ERROR *** Detach databuffer Failed";
	int		stor1_off = 2;
	int		stor2_off = 12;
	int		*stor1p;
	int		*stor2p;

	stor1p = messp + stor1_off;
	stor2p = messp + stor2_off;

	      		
	if ( *stor1p > 0 ) {
	   close ( *stor1p );
	}
	if ( *stor2p > 0 ) {
	   close ( *stor2p );
	} 

	/* Detach shared memory */
	if ( shmdt( messp ) == -1) {
           errprint("%s\n",err2);
	}
	
	remove("/Applications/sirius/reduc/system/reduc.lock");
	exit(0);
}


/* --------------------------------------------------------------------------- */
void options_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
    	Widget  dialog, toggle;
    	Arg 	args[5];
    	int 	n = 0;
	int	tapetype_off = 3;
	int	*typep;
	extern void select_pushed();


   	XmString t = XmStringCreateLocalized ("Select Input Tape Format:");
    	XmString btn1 = XmStringCreateLocalized ("SIRIUS tape format");
    	XmString btn2  = XmStringCreateLocalized ("DAISY tape format");

	typep = messp + tapetype_off;

	/* Create the dialog, return to previous select state */
    	XtSetArg (args[n], XmNautoUnmanage, False); n++; 
    	dialog = XmCreateQuestionDialog (MainWindow, "Format", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, (void *)XtDestroyWidget, NULL); 
  	XtUnmanageChild (XmMessageBoxGetChild (dialog, XmDIALOG_HELP_BUTTON)); 
  	XtUnmanageChild (XmMessageBoxGetChild (dialog, XmDIALOG_CANCEL_BUTTON)); 

	if (*typep/10 == 0) {
	   toggle = XmVaCreateSimpleRadioBox ( dialog, "toggle", 
						0, 
						select_pushed, 
						XmVaRADIOBUTTON, btn1, NULL, NULL, NULL,
						XmVaRADIOBUTTON, btn2, NULL, NULL, NULL,
						NULL);
	}
	
	if (*typep/10 == 1) {
	   toggle = XmVaCreateSimpleRadioBox ( dialog, "toggle", 
						1, 
						select_pushed, 
						XmVaRADIOBUTTON, btn1, NULL, NULL, NULL,
						XmVaRADIOBUTTON, btn2, NULL, NULL, NULL,
						NULL);
	}


	XmStringFree (btn1);
	XmStringFree (btn2);
	XtManageChild (toggle);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);
		
}
void select_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{
	int		tapetype_off = 3;
	int		*typep;
        int             i;
	int	n = (int) client_data;
    	XmToggleButtonCallbackStruct *cbs =  (XmToggleButtonCallbackStruct *) call_data;

	typep = messp + tapetype_off;
	if ( cbs->set == False)
	   return;
        i = *typep;
	if (n == 0) {    /* SIRIUS input */
	   *typep = 0*10 + (i - (i/10) * 10);
	} 
	if (n == 1) {    /* DAISY input */
	   *typep = 1*10 + (i - (i/10) * 10);
	}

}

/* --------------------------------------------------------------------------- */
void sortwhole_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{

	int		files2do_off = 7;
        int		recs2do_off  = 8;
	int		*files2dop;
	int		*recs2dop;

	int	status;
	char	err[1024] = "*** ERROR *** Already started !";

	files2dop = messp + files2do_off;
	recs2dop  = messp + recs2do_off;

	/* Update Message_Box slots for files2do and recs2do */
	*files2dop = 0;
	*recs2dop  = 0;

        if ( Is_Started == 0)
	   status = reduc_startshift( );
	else
	   errprint("%s\n",err);	
	   
	if ( status == -1 ) {
	   errprint("%s\n",err);	
	   Is_Started = 0;
	} else {
	   Is_Started = 1;
	}
	
	status = set_pushbutton_status();

}
/* --------------------------------------------------------------------------- */
void sortfile_pushed(Widget widget, XtPointer client_data, XtPointer call_data)


{

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Enter Number of Files to Sort:");
    	Arg 	args[5];
    	int 	n = 0;

    	extern void get_fileno( );


	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filenumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, get_fileno, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);

}

void get_fileno( Widget widget, XtPointer client_data, XtPointer call_data )
{

	int		files2do_off = 7;
	int		recs2do_off  = 8;
	int		*files2dop;
	int		*recs2dop;

	char 		*cfile;
	int		status;
	int		fileno;
	int		recno;
	char		err[1024] = "*** ERROR *** Already started !";

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	fileno = atoi( cfile );

	/* Update Message_Box slots for files2do and recs2do */
	files2dop = messp + files2do_off;
	recs2dop  = messp + recs2do_off;
	*files2dop = fileno;
	*recs2dop  = 0;


        if ( Is_Started == 0)
	   status = reduc_startshift( ); 
	else
	   errprint("%s\n",err); 	
	   
	if ( status == -1 ) 
	   Is_Started = 0;
	else
	   Is_Started = 1;
	
	status = set_pushbutton_status();

}

/* --------------------------------------------------------------------------- */
void sortrecs_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Enter Number of Records to Sort:");
    	Arg 	args[5];
    	int 	n = 0;

    	extern void get_recno( );


	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filenumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, get_recno, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);

}
void get_recno( Widget widget, XtPointer client_data, XtPointer call_data )
{

	int		files2do_off = 7;
        int		recs2do_off  = 8;
	int		*files2dop;
	int		*recs2dop;

	char 		*cfile;
	int		status;
	int		recno;
	char		err[1024] = "*** ERROR *** Already started !";

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	recno = atoi( cfile );

	/* Update Message_Box slots for files2do and recs2do */
	files2dop  = messp + files2do_off;
	recs2dop   = messp + recs2do_off;
	*files2dop = 0;
	*recs2dop  = recno;


        if ( Is_Started == 0)
	   status = reduc_startshift( ); 
	else
	   errprint("%s\n",err); 	
	   
	if ( status == -1 ) 
	   Is_Started = 0;
	else
	   Is_Started = 1;
	
	status = set_pushbutton_status( );

}

/* --------------------------------------------------------------------------- */
void reductwhole_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{

	int		files2do_off = 7;
        int		recs2do_off  = 8;
	int		*files2dop;
	int		*recs2dop;

	int	status;
	char	err[1024] = "*** ERROR *** Already started !";

	files2dop = messp + files2do_off;
	recs2dop  = messp + recs2do_off;

	/* Update Message_Box slots for files2do and recs2do */
	*files2dop = 0;
	*recs2dop  = 0;

        if ( Is_Started == 0)
	   status = reduc_startwin( );
	else
	   errprint("%s\n",err);	
	   
	if ( status == -1 ) 
	   Is_Started = 0;
	else
	   Is_Started = 1;
	
	status = set_pushbutton_status();

}
/* --------------------------------------------------------------------------- */
void reductfile_pushed(Widget widget, XtPointer client_data, XtPointer call_data)


{

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Enter Number of Files to Sort:");
    	Arg 	args[5];
    	int 	n = 0;

    	extern void win_fileno( );


	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filenumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, win_fileno, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);

}

void win_fileno( Widget widget, XtPointer client_data, XtPointer call_data )
{

	int		files2do_off = 7;
	int		recs2do_off  = 8;
	int		*files2dop;
	int		*recs2dop;

	char 		*cfile;
	int		status;
	int		fileno;
	int		recno;
	char		err[1024] = "*** ERROR *** Already started !";

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	fileno = atoi( cfile );

	/* Update Message_Box slots for files2do and recs2do */
	files2dop = messp + files2do_off;
	recs2dop  = messp + recs2do_off;
	*files2dop = fileno;
	*recs2dop  = 0;


        if ( Is_Started == 0)
	   status = reduc_startwin( );
	else
	   errprint("%s\n",err); 	
	   
	if ( status == -1 ) 
	   Is_Started = 0;
	else
	   Is_Started = 1;
	
	status = set_pushbutton_status();

}

/* --------------------------------------------------------------------------- */
void reductrecs_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Enter Number of Records to Sort:");
    	Arg 	args[5];
    	int 	n = 0;

    	extern void win_recno( );


	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filenumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, win_recno, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);

}

void win_recno( Widget widget, XtPointer client_data, XtPointer call_data )
{

	int		files2do_off = 7;
	int		recs2do_off  = 8;
	int		*files2dop;
	int		*recs2dop;

	char 		*cfile;
	int		status;
	int		recno;
	char		err[1024] = "*** ERROR *** Already started !";

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	recno = atoi( cfile );

	/* Update Message_Box slots for files2do and recs2do */
	files2dop = messp + files2do_off;
	recs2dop  = messp + recs2do_off;
	*files2dop = 0;
	*recs2dop  = recno;


        if ( Is_Started == 0)
	   status = reduc_startwin( );
	else
	   errprint("%s\n",err); 	
	   
	if ( status == -1 ) 
	   Is_Started = 0;
	else
	   Is_Started = 1;
	
	status = set_pushbutton_status( );

}


/* --------------------------------------------------------------------------- */
void stop_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	char	err[1024] = "*** ERROR *** Not started !";

        if ( Is_Started == 1)
	   status = reduc_stop( );
	else
	   errprint("%s\n",err);	
	   
	if ( status == -1 ) 
	   Is_Started = 1;
	else
	   Is_Started = 0;

	status = set_pushbutton_status( );

}

/*--------------------------------------------------------------------------- */
void status_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int status;
	
	status = reduc_status( );
	status = set_pushbutton_status( );

}




/*--------------------------------------------------------------------------- */
void info_pushed(Widget w, XtPointer client_data, XtPointer call_data)

{

	
	int status;
    	XmDrawnButtonCallbackStruct *cbs = 
        (XmDrawnButtonCallbackStruct *) call_data;
    	if (cbs->reason == XmCR_ACTIVATE) {
	   status = reduc_status( );
	   status = set_pushbutton_status( );
    	}

    	else if (cbs->reason == XmCR_EXPOSE) {
           Dimension ht, st;

           XtVaGetValues (	w, 
            			XmNhighlightThickness, &ht,
            			XmNshadowThickness, &st,
            			NULL);

        XtVaSetValues 	(	w,
            			XmNwidth, 2 * ht + 2 * st + 26,
            			XmNheight, 2 * ht + 2 * st + 26,
            			NULL);

        XCopyArea 	(	XtDisplay (w), pixmap1, XtWindow (w), 
            			XDefaultGCOfScreen (XtScreen (w)), -8, -2, 26, 26, 
            			ht + st, ht + st);
    	}
}



/*--------------------------------------------------------------------------- */
void none_device1_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 0;
	int	inout  = 1; 

	status = reduc_storage( device, inout );
	if (status == 0) {
	   Media1_On = 0;
	   Input_drive_no = 0;
	}

	status = set_pushbutton_status( );

}

/*--------------------------------------------------------------------------- */
void exb1_device1_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 1;
	int	inout  = 1; 

	status = reduc_storage( device, inout );
	if ( status == 0 ) {
	   Media1_On = 1;
	   Input_drive_no = 1;
	}

	status = set_pushbutton_status( );

}

/*--------------------------------------------------------------------------- */
void exb2_device1_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 2;
	int	inout  = 1; 

	status = reduc_storage( device, inout );
	if ( status == 0 ) {
	   Media1_On = 1;
	   Input_drive_no = 2;
	}

	status = set_pushbutton_status( );

}

/*--------------------------------------------------------------------------- */
void disc_device1_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 3;
	int	inout  = 1; 

	status = reduc_storage( device, inout );
	if ( status == 0 ) {
	   Media1_On = 1;
	   Input_drive_no = 3;
	}

	status = set_pushbutton_status( );

}


/*--------------------------------------------------------------------------- */
void none_device2_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 0;
	int	inout  = 2; 

	status = reduc_storage( device, inout );
	if (status == 0) {
	   Media2_On = 0;
	   Output_drive_no = 0;
	}

	status = set_pushbutton_status( );

}

/*--------------------------------------------------------------------------- */
void exb1_device2_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 1;
	int	inout  = 2; 

	status = reduc_storage( device, inout );
	if ( status == 0 ) {
	   Media2_On = 1;
	   Output_drive_no = 1;
	}

	status = set_pushbutton_status( );

}

/*--------------------------------------------------------------------------- */
void exb2_device2_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 2;
	int	inout  = 2; 

	status = reduc_storage( device, inout );
	if ( status == 0 ) {
	   Media2_On = 1;
	   Output_drive_no = 2;
	}

	status = set_pushbutton_status( );

}

/*--------------------------------------------------------------------------- */
void disc_device2_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int	device = 3;
	int	inout  = 2; 

	status = reduc_storage( device, inout );
	if ( status == 0 ) {
	   Media2_On = 1;
	   Output_drive_no = 3;
	}

	status = set_pushbutton_status( );

}


/*--------------------------------------------------------------------------- */
void current1_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int 	cup  	= 1;
	int	bot  	= 0;
	int	eod    	= 0;
	int	fileno 	= 0;
	int	recno 	= 0;
	int	inout   = 1; 

	char	err1[1024]= "*** ERROR *** Could not start at current position";

	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
 	   errprint("%s\n", err1);
	}


}

/*--------------------------------------------------------------------------- */
void bot1_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int 	cup  	= 0;
	int	bot  	= 1;
	int	eod    	= 0;
	int	fileno 	= 0;
	int	recno 	= 0;
	int	inout   = 1; 

	char	err1[1024]= "*** ERROR *** Could not position to BOT";

	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
	   errprint("%s\n", err1);
	}
}


/*--------------------------------------------------------------------------- */
void file1_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Enter File Number:");
    	Arg 	args[5];
    	int 	n = 0;

    	extern void read_fileno( );


	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filenumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, read_fileno, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);

}

void read_fileno( Widget widget, XtPointer client_data, XtPointer call_data )
{
	char 	*cfile;
	char	err1[1024]= "*** ERROR *** Could not position to file ";
	int	status;
	int 	cup  	= 0;
	int	bot  	= 0;
	int	eod    	= 0;
	int	fileno 	= 0;
	int	recno 	= 0;
	int	inout   = 1; 

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	fileno = atoi( cfile );
	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
 	   errprint("%s%d\n", err1, fileno);
	}

}

/*--------------------------------------------------------------------------- */
void record1_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Enter Record Number:");
    	Arg 	args[5];
    	int 	n = 0;

    	extern void read_recordno( );


	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Recordnumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, read_recordno, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);

}

void read_recordno( Widget widget, XtPointer client_data, XtPointer call_data )
{
	char 	*cfile;
	char	err1[1024]= "*** ERROR *** Could not position to record ";
	int	status;
	int 	cup  	= 0;
	int	bot  	= 0;
	int	eod    	= 0;
	int	fileno 	= -1;
	int	recno 	= 0;
	int	inout   = 1; 

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	recno = atoi( cfile );
	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
 	   errprint("%s%d\n", err1, recno);
	}

}

/*--------------------------------------------------------------------------- */
void current2_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int 	cup  	= 1;
	int	bot  	= 0;
	int	eod    	= 0;
	int	fileno 	= 0;
	int	recno 	= 0;
	int	inout   = 2; 

	char	err1[1024]= "*** ERROR *** Could not start at current position";

	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
 	   errprint("%s\n", err1);
	}


}

/*--------------------------------------------------------------------------- */
void bot2_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int 	cup  	= 0;
	int	bot  	= 1;
	int	eod    	= 0;
	int	fileno 	= 0;
	int	recno 	= 0;
	int	inout   = 2; 

	char	err1[1024]= "*** ERROR *** Could not position to BOT";

	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
	   errprint("%s\n", err1);
	}
}

/*--------------------------------------------------------------------------- */
void eod2_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	int 	cup  	= 0;
	int	bot  	= 0;
	int	eod    	= 1;
	int	fileno 	= 0;
	int	recno 	= 0;
	int	inout   = 2; 

	char	err1[1024]= "*** ERROR *** Could not position to EOD";

	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
 	   errprint("%s\n", err1);
	}
	
}

/*--------------------------------------------------------------------------- */
void file2_position_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Enter File Number:");
    	Arg 	args[5];
    	int 	n = 0;

    	extern void read2_fileno( );


	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filenumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, read2_fileno, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);

   	XtPopup (XtParent (dialog), XtGrabNone);

}

void read2_fileno( Widget widget, XtPointer client_data, XtPointer call_data )
{
	char 	*cfile;
	char	err1[1024]= "*** ERROR *** Could not position to file ";
	int	status;
	int 	cup  	= 0;
	int	bot  	= 0;
	int	eod    	= 0;
	int	fileno 	= 0;
	int	recno 	= 0;
	int	inout   = 2; 

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	fileno = atoi( cfile );
	status = reduc_position( cup, bot, eod, fileno, recno, inout );
	if ( status == -1 ) {
 	   errprint("%s%d\n", err1, fileno);
	}

}


/*--------------------------------------------------------------------------- */
void unload1_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{
	int	status;
	int	inout  = 1; 
	char	err[1024] = "*** ERROR *** Stop reduc first !";

        if ( Is_Started == 0)
	   status = reduc_unload( inout );
	else
	   errprint("%s\n",err);
	
	Media1_On = 0;	   
	status = set_pushbutton_status( );

}


/*--------------------------------------------------------------------------- */
void unload2_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{
	int	status;
	int	inout  = 2; 
	char	err[1024] = "*** ERROR *** Stop reduc first !";

        if ( Is_Started == 0)
	   status = reduc_unload( inout );
	else
	   errprint("%s\n",err);
	
	Media2_On = 0;	   
	status = set_pushbutton_status( );

}


/*--------------------------------------------------------------------------- */
void shiftinit_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	FILE	*ifp;
	FILE	*ofp;
	const char infile[]  = "/Applications/sirius/reduc/data/gainshift.init";
	const char outfile[] = "/Applications/sirius/reduc/data/gainshift.tmp";

	char	err5[1024] = "*** ERROR *** Could not open source file";
	char	err6[1024] = "*** ERROR *** Could not open destination file";

	extern void   get_file();

	/* Open files ... */
	if (( ifp = fopen(infile,"r")) == NULL) {
	   errprint("%s\n",err5);
	   return;
	}
	if (( ofp = fopen(outfile,"w"))  == NULL) {
	   errprint("%s\n",err6);
	   return;
	}

	/* Copy content of input file to output */
	filecopy (ifp, ofp);
	fclose(ifp);
	fclose(ofp);

}


/*--------------------------------------------------------------------------- */
void shiftfile_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	char	err[1024] = "*** ERROR *** Stop acquisition first !";

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("File Name:");
    	Arg 	args[5];
    	int 	n = 0;

	extern void   get_file();

   
	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Shiftfile", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, get_file, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);
        XtPopup (XtParent (dialog), XtGrabNone);

}

void get_file( Widget widget, XtPointer client_data, XtPointer call_data)
{
	FILE	*ifp;
	FILE	*ofp;
    	char 	*filename;
	int	filestat;
	const char outfile[] = "/Applications/sirius/reduc/data/gainshift.tmp";
	char 	string[128];
	char	err1[1024] = "*** ERROR *** No file selected";
	char	err2[1024] = "*** ERROR *** File not found";
	char	err3[1024] = "*** ERROR *** File is a directory";
	char	err4[1024] = "*** ERROR *** File is not readable";
	char	err5[1024] = "*** ERROR *** Could not open source file";
	char	err6[1024] = "*** ERROR *** Could not open destination file";

    	Widget 	push_button = (Widget) client_data;  

   	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &filename))
        	return; /* must have been an internal error */


    	if (!*filename) { /* nothing typed? */
	   errprint("%s\n",err1);
           XtFree( filename); /* even "" is an allocated byte */
           return;
    	}

	/* Check file for exsistence, not directory, and read access */
	filestat = is_writable(filename);
	if ( filestat == -1 ) {
	   errprint("%s\n",err2);
	   return;
	}
	else if ( filestat == -2 ) {
	   errprint("%s\n",err3);
	   return;
	}
	else if ( filestat == -3 ) {
	   errprint("%s\n",err4);
	   return;
	}
	   
	/* Open files ... */
	if (( ifp = fopen(filename,"r")) == NULL) {
	   errprint("%s\n",err5);
	   return;
	}
	if (( ofp = fopen(outfile,"w"))  == NULL) {
	   errprint("%s\n",err6);
	   return;
	}


	/* Copy content of input file to output */
	filecopy (ifp, ofp);
	fclose(ifp);
	fclose(ofp);

    	XtFree (filename);
    	XtDestroyWidget(widget);
}

/*--------------------------------------------------------------------------- */
void shiftshow_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
    	char	buf[1024];
	char	c;
	int	i;
	int	buflength = 1024;
    	FILE	*routine;

	routine = fopen( "/Applications/sirius/reduc/data/gainshift.tmp","r" );

	for (i=0; i<buflength-1 && (c = getc(routine)) && c != EOF; i++)
	   buf[i] = toascii(c);
	buf[i] = '\0';

        fclose( routine );
	
	wprint("Gain and shift values :\n");
    	wprint("%s\n", buf);
}

/*--------------------------------------------------------------------------- */
void shiftadjust_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("Adjust shift every 100th (default) record:");
/*    	XmString t = XmStringCreateLocalized ("Shift Adjustment factor [10] :");       Magne */
    	Arg 	args[5];
    	int 	n = 0;

	extern void   get_fac();

   
	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Shiftadjustment", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, get_fac, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);
        XtPopup (XtParent (dialog), XtGrabNone);

}

void get_fac( Widget widget, XtPointer client_data, XtPointer call_data )
{
	char 	*cfile;
	char	err[1024] = "*** ERROR *** Could not set shift adjustment factor";
	int	status;
	int 	shift = 100;

    	Widget push_button = (Widget) client_data;  
    	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &cfile))
        	return; /* must have been an internal error */

    	XtDestroyWidget(widget);


	shift = atoi( cfile );
	if (shift < 10 ) shift = 10;
	if (shift <= 0 ) shift = 100;

	status = reduc_shift( shift );
	if ( status == -1 ) {
 	   errprint("%s%d\n", err, shift);
	}
}


/*--------------------------------------------------------------------------- */
void peakinit_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	FILE	*ifp;
	FILE	*ofp;
	const char infile[]  = "/Applications/sirius/reduc/data/peak.init";
	const char outfile[] = "/Applications/sirius/reduc/data/peak.tmp";

	char	err5[1024] = "*** ERROR *** Could not open source file";
	char	err6[1024] = "*** ERROR *** Could not open destination file";

	extern void   get_file();

	/* Open files ... */
	if (( ifp = fopen(infile,"r")) == NULL) {
	   errprint("%s\n",err5);
	   return;
	}
	if (( ofp = fopen(outfile,"w"))  == NULL) {
	   errprint("%s\n",err6);
	   return;
	}

	/* Copy content of input file to output */
	filecopy (ifp, ofp);
	fclose(ifp);
	fclose(ofp);

}


/*--------------------------------------------------------------------------- */
void peakfile_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;
	char	err[1024] = "*** ERROR *** Stop reduc first !";

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("File Name:");
    	Arg 	args[5];
    	int 	n = 0;

	extern void   get_peakfile();

   
	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filenumber", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, get_peakfile, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);
        XtPopup (XtParent (dialog), XtGrabNone);

}


void get_peakfile( Widget widget, XtPointer client_data, XtPointer call_data)
{
	FILE	*ifp;
	FILE	*ofp;
    	char 	*filename;
	int	filestat;
	const char outfile[] = "/Applications/sirius/reduc/data/peak.tmp";
	char 	string[128];
	char	err1[1024] = "*** ERROR *** No file selected";
	char	err2[1024] = "*** ERROR *** File not found";
	char	err3[1024] = "*** ERROR *** File is a directory";
	char	err4[1024] = "*** ERROR *** File is not readable";
	char	err5[1024] = "*** ERROR *** Could not open source file";
	char	err6[1024] = "*** ERROR *** Could not open destination file";

    	Widget 	push_button = (Widget) client_data;  

   	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &filename))
        	return; /* must have been an internal error */


    	if (!*filename) { /* nothing typed? */
	   errprint("%s\n",err1);
           XtFree( filename); /* even "" is an allocated byte */
           return;
    	}

	/* Check file for exsistence, not directory and read access */
	filestat = is_writable(filename);
	if ( filestat == -1 ) {
	   errprint("%s\n",err2);
	   return;
	}
	else if ( filestat == -2 ) {
	   errprint("%s\n",err3);
	   return;
	}
	else if ( filestat == -3 ) {
	   errprint("%s\n",err4);
	   return;
	}
	   
	/* Open files ... */
	if (( ifp = fopen(filename,"r")) == NULL) {
	   errprint("%s\n",err5);
	   return;
	}
	if (( ofp = fopen(outfile,"w"))  == NULL) {
	   errprint("%s\n",err6);
	   return;
	}


	/* Copy content of input file to output */
	filecopy (ifp, ofp);
	fclose(ifp);
	fclose(ofp);

    	XtFree (filename);
    	XtDestroyWidget(widget);
}


/*--------------------------------------------------------------------------- */
void peakshow_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
    	char	buf[1024];
	char	c;
	int	i;
	int	buflength = 1024;
    	FILE	*routine;

	routine = fopen( "/Applications/sirius/reduc/data/peak.tmp","r" );

	for (i=0; i<buflength-1 && (c = getc(routine)) && c != EOF; i++)
	   buf[i] = toascii(c);
	buf[i] = '\0';

        fclose( routine );	
    	
	wprint("Target peak values :\n");
    	wprint("%s\n", buf);
}

/*--------------------------------------------------------------------------- */
void resultshow_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	char	buf[1024];
	char	c;
	int	i;
	int	buflength = 1024;
	FILE	*routine;

	routine = fopen( "/Applications/sirius/reduc/data/result.out","r" );
	for (i=0; i<buflength-1 && (c = getc(routine)) && c != EOF; i++)
	   buf[i] = toascii(c);
	buf[i] = '\0';
        fclose( routine );	    	
    	wprint("%s\n", buf);
}

/*--------------------------------------------------------------------------- */
void peak_pushed(Widget w, XtPointer client_data, XtPointer call_data)

{
	char	buf[1024];
	char	c;
	int	i;
	int	buflength = 1024;
	FILE	*routine;	
	int 	status;

    	XmDrawnButtonCallbackStruct *cbs = 
        (XmDrawnButtonCallbackStruct *) call_data;

    	if (cbs->reason == XmCR_ACTIVATE) {
	   routine = fopen( "/Applications/sirius/reduc/data/result.out","r" );
	   for (i = 0; i < buflength && (c = getc(routine)) && c != EOF; i++)
	      buf[i] = toascii(c);
	   buf[i] = '\0';
           fclose( routine );	    	
    	   wprint("%s\n", buf);
    	}

    	else if (cbs->reason == XmCR_EXPOSE) {
           Dimension ht, st;

           XtVaGetValues (	w, 
            			XmNhighlightThickness, &ht,
            			XmNshadowThickness, &st,
            			NULL);

           XtVaSetValues 	(	w,
            			XmNwidth, 2 * ht + 2 * st + 26,
            			XmNheight, 2 * ht + 2 * st + 26,
            			NULL);

          XCopyArea 	(	XtDisplay (w), pixmap2, XtWindow (w), 
            			XDefaultGCOfScreen (XtScreen (w)), -5, -5, 26, 26, 
            			ht + st, ht + st);
    	}
}



/*--------------------------------------------------------------------------- */
void winfile_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
	int	status;

    	Widget 	dialog;
    	XmString t = XmStringCreateLocalized ("File Name:");
    	Arg 	args[5];
    	int 	n = 0;

	extern void   get_winfile();

   
	/* Create the dialog */
    	XtSetArg (args[n], XmNselectionLabelString, t); n++;
    	XtSetArg (args[n], XmNautoUnmanage, False); n++;

    	dialog = XmCreatePromptDialog (MainWindow, "Filename", args, n);
    	XmStringFree (t); /* always destroy compound strings when done */
    	XtAddCallback (dialog, XmNokCallback, get_winfile, widget);
   	XtAddCallback (dialog, XmNcancelCallback, (void *)XtDestroyWidget, NULL);
    	XtSetSensitive (XmSelectionBoxGetChild (dialog, XmDIALOG_HELP_BUTTON), False);
    	XtManageChild (dialog);
        XtPopup (XtParent (dialog), XtGrabNone);

}

void get_winfile( Widget widget, XtPointer client_data, XtPointer call_data)
{
	FILE	*ifp;
	FILE	*ofp;
    	char 	*filename;
	int	filestat;
	const char outfile[] = "/Applications/sirius/reduc/data/win.tmp";
	char 	string[128];
	char	err1[1024] = "*** ERROR *** No file selected";
	char	err2[1024] = "*** ERROR *** File not found";
	char	err3[1024] = "*** ERROR *** File is a directory";
	char	err4[1024] = "*** ERROR *** File is not readable";
	char	err5[1024] = "*** ERROR *** Could not open source file";
	char	err6[1024] = "*** ERROR *** Could not open destination file";

	void	filecopy(FILE *,FILE *);

    	Widget 	push_button = (Widget) client_data;  

   	XmSelectionBoxCallbackStruct *cbs =  (XmSelectionBoxCallbackStruct *) call_data;
	
    	if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &filename))
        	return; /* must have been an internal error */


    	if (!*filename) { /* nothing typed? */
	   errprint("%s\n",err1);
           XtFree( filename); /* even "" is an allocated byte */
           return;
    	}

	/* Check file for exsistence, not directory and read access */
	filestat = is_writable(filename);
	if ( filestat == -1 ) {
	   errprint("%s\n",err2);
	   return;
	}
	else if ( filestat == -2 ) {
	   errprint("%s\n",err3);
	   return;
	}
	else if ( filestat == -3 ) {
	   errprint("%s\n",err4);
	   return;
	}
	   
	/* Open files ... */
	if (( ifp = fopen(filename,"r")) == NULL) {
	   errprint("%s\n",err5);
	   return;
	}
	if (( ofp = fopen(outfile,"w"))  == NULL) {
	   errprint("%s\n",err6);
	   return;
	}


	/* Copy content of input file to output */
	filecopy (ifp, ofp);
	fclose(ifp);
	fclose(ofp);

    	XtFree (filename);
    	XtDestroyWidget(widget);
}



/*--------------------------------------------------------------------------- */
void winshow_pushed(Widget widget, XtPointer client_data, XtPointer call_data)

{
    	char	buf[1024];
	char	c;
	int	i;
	int	buflength = 1024;
    	FILE	*routine;

	routine = fopen( "/Applications/sirius/reduc/data/win.tmp","r" );

	for (i=0; i<buflength-1 && (c = getc(routine)) && c != EOF; i++)
	   buf[i] = toascii(c);
	buf[i] = '\0';

        fclose( routine );	
    	
    	wprint("%s\n\n", buf);
}


/*--------------------------------------------------------------------------- */
void h_about_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{


	Widget dialog;
	XmString text;
	Arg args[5];
    	Pixel		fg, bg;
    
	XtVaGetValues (	MainWindow,
        		XmNforeground, &fg,
        		XmNbackground, &bg,
        		NULL);

        pixmap = XmGetPixmap (XtScreen (newcol), "/Applications/sirius/bitmap/logo", fg, bg);

	text = XmStringCreateLtoR ("          Welcome to REDUC  1.0\n\n         Oslo Cyclotron Laboratory\n Data Reduction and Alignment System\n\n           tore.ramsoy@nrpa.no\n       magne.guttormsen@fys.uio.no\n                Oslo February 2007\n",XmFONTLIST_DEFAULT_TAG);


	XtSetArg ( args[0], XmNmessageString, text); 
	dialog = XmCreateInformationDialog (MainWindow,"about",args,1);

	XmStringFree ( text );
	XtUnmanageChild ( XmMessageBoxGetChild( dialog, XmDIALOG_CANCEL_BUTTON) );
	XtUnmanageChild ( XmMessageBoxGetChild( dialog, XmDIALOG_HELP_BUTTON) );
	/*XtUnmanageChild ( XmMessageBoxGetChild( dialog, XmDIALOG_SYMBOL_LABEL) ); */
	XtVaSetValues(dialog, XmNsymbolPixmap,pixmap,NULL); 

	XtManageChild( dialog );
	XtPopup ( XtParent(dialog), XtGrabNone);
 

}


/*--------------------------------------------------------------------------- */

void h_install_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{
   system("nedit /Applications/sirius/reduc/help/README &");  /* for Linux*/
/*   system("open /Applications/sirius/reduc/help/README &");  for Mac */
}


/*--------------------------------------------------------------------------- */

void h_reduc_pushed(Widget widget, XtPointer client_data, XtPointer call_data)
{
   system("firefox http://ocl.uio.no/reduc/ &");
/*   system("open http://ocl.uio.no/reduc/ &");  for Mac */
}



/* ============================================================================*/
/*                  This section contains utility routines                     */
/* ============================================================================*/


/* Update status for all menu options and pushbuttons according to task state */
int set_pushbutton_status( )
{

	int		Media1_On = 0;
	int		Media2_On = 0;
	int		stor1_off = 2;
	int		stor2_off = 12;
	int		offl_off  = 4;
	int		pass_off  = 19;
	int		type_off  = 3;
	int		*stor1p;
	int		*stor2p;
	int		*offlp;
	int		*passp;
	int      *typep;

	stor1p = messp + stor1_off;
	stor2p = messp + stor2_off;
	offlp  = messp + offl_off;
	passp  = messp + pass_off;
	typep  = messp + type_off;

	if ( *stor1p > 0 ) {
	   Media1_On = 1;
	} 
	if ( *stor2p > 0 ) {
	   Media2_On = 1;
	} 

	if ( *stor1p > 0 && *typep/10 == 3 ) {    /* Disk is chosen as input */
  	   XtSetSensitive( file_options,        False );
	} else { 
  	   XtSetSensitive( file_options,        True  );
	} 

	if ( *offlp == 1 ) {
	   Is_Started = 1;
	} 
	if ( *offlp == 0 ) {
	   Is_Started = 0;
	} 


	/* Pass 1 (shift extraction) or pass2 (reduction) ? */
	if ( *passp == 1 ) {
  	   XtSetSensitive( peakb, 		True  );
	   XtSetSensitive( result_show,	        True  );
        }

	if ( *passp == 2 ) {
  	   XtSetSensitive( peakb, 		False );
	   XtSetSensitive( result_show,	        False );
	}
	   

	/* Running ... */
 	if ( Is_Started == 1 ) {
  	   XtSetSensitive( file_quit, 		False );
  	   XtSetSensitive( run_sort, 		False );
	   XtSetSensitive( run_reduct,		False );
  	   XtSetSensitive( run_stop,  		True  );
  	   XtSetSensitive( tape1_device,  	False );
  	   XtSetSensitive( tape1_position,  False );
  	   XtSetSensitive( tape1_unload,  	False );
  	   XtSetSensitive( shift_init,  	    False );
  	   XtSetSensitive( shift_file,  	    False );
  	   XtSetSensitive( shift_adjust,  	False );
  	   XtSetSensitive( peak_init,  	    False );
  	   XtSetSensitive( peak_file,  	    False );
  	   XtSetSensitive( win_file,  		False );
	}

	/* Stopped ... */
 	if ( Is_Started == 0 ) {
/*
 printf("Media1_On,Media2_On,Input_drive_no,Output_drive_no:  %d %d %d %d \n",Media1_On,Media2_On,Input_drive_no,Output_drive_no);
*/
  	   XtSetSensitive( file_quit, 		True  );
  	   XtSetSensitive( run_sort, 		False );
	   XtSetSensitive( run_reduct,		False );
  	   XtSetSensitive( run_stop,  		False );
  	   XtSetSensitive( tape1_device,  	True  );
  	   XtSetSensitive( tape1_position,  False );
  	   XtSetSensitive( tape1_unload,  	False );
	   XtSetSensitive( device1_exb1,	True  );
	   XtSetSensitive( device1_exb2,	True  );
	   XtSetSensitive( device1_disc,	True  );
  	   XtSetSensitive( tape2_device,  	True  );
  	   XtSetSensitive( tape2_position,  False );
  	   XtSetSensitive( tape2_unload,  	False );
	   XtSetSensitive( device2_exb1,	True  );
	   XtSetSensitive( device2_exb2,	True  );
	   XtSetSensitive( device2_disc,	True  );
  	   XtSetSensitive( shift_init,  	    True  );
  	   XtSetSensitive( shift_file,  	    True  );
  	   XtSetSensitive( shift_adjust,  	True  );
  	   XtSetSensitive( peak_init,  	    True  );
  	   XtSetSensitive( peak_file,  	    True  );
  	   XtSetSensitive( win_file,  		True  );

	   if ( Media1_On == 1 && Media2_On == 0 ) {

  	      XtSetSensitive( tape1_unload,  	True  );
  	      XtSetSensitive( tape1_position,  	True  );
	      XtSetSensitive( tape2_unload,  	False );
  	      XtSetSensitive( tape2_position,  	False );
	      XtSetSensitive( device1_exb1,     False );
	      XtSetSensitive( device1_exb2,		False );
	      XtSetSensitive( device1_disc,		False );

	      if (Input_drive_no == 1 ) {
	      XtSetSensitive( device2_exb1,			False );
              XtSetSensitive( device2_exb2,     True  );
              XtSetSensitive( device2_disc,     True  );
	      }
	      if (Input_drive_no == 2 ) {
	      XtSetSensitive( device2_exb1,			True  );
              XtSetSensitive( device2_exb2,     False );
              XtSetSensitive( device2_disc,     True  );
	      }
              if (Input_drive_no == 3 ) {
	      XtSetSensitive( device2_exb1,			True  );
              XtSetSensitive( device2_exb2,     True  );
              XtSetSensitive( device2_disc,     True  );
	      }

	   }


	   if ( Media2_On == 1 && Media1_On == 0 ) {
  	      XtSetSensitive( tape2_unload,  	True  );
  	      XtSetSensitive( tape2_position,  	True  );
	      XtSetSensitive( tape1_unload,  	False );
  	      XtSetSensitive( tape1_position,  	False );
	      XtSetSensitive( device2_exb1,		False );
	      XtSetSensitive( device2_exb2,		False );
	      XtSetSensitive( device2_disc,		False );

	      if (Output_drive_no == 1 ) {
	      XtSetSensitive( device1_exb1,			False );
              XtSetSensitive( device1_exb2,     True  );
              XtSetSensitive( device1_disc,     True  );
	      }
	      if (Output_drive_no == 2 ) {
	      XtSetSensitive( device1_exb1,			True  );
              XtSetSensitive( device1_exb2,     False );
              XtSetSensitive( device1_disc,     True  );
	      }
	      if (Output_drive_no == 3 ) {
	      XtSetSensitive( device1_exb1,			True  );
              XtSetSensitive( device1_exb2,     True  );
              XtSetSensitive( device1_disc,     True  );
	      }

	   }

	   if ( Media1_On == 1 ) {
	      XtSetSensitive( run_sort,		True  );
	   }

	   if ( Media2_On == 1 ) {
	      XtSetSensitive( run_reduct,	True  );
	   }

	   if ( Media1_On == 1 && Media2_On == 1) {
  	      XtSetSensitive( tape2_unload,  	True  );
  	      XtSetSensitive( tape2_position,  	True  );
	      XtSetSensitive( tape1_unload,  	True  );
  	      XtSetSensitive( tape1_position,  	True  );
	      XtSetSensitive( device1_exb1,		False );
	      XtSetSensitive( device1_exb2,		False );
	      XtSetSensitive( device1_disc,		False );
	      XtSetSensitive( device2_exb1,		False );
	      XtSetSensitive( device2_exb2,		False );
	      XtSetSensitive( device2_disc,		False );
  	      XtSetSensitive( run_sort,			True  );
	      XtSetSensitive( run_reduct,		True  );

	   }	      
	}


	return 0;
}

/*--------------------------------------------------------------------------- */
/* Copy a file from ifp to ofp */
void filecopy( FILE *ifp, FILE *ofp)
{
	int	c;
	while ((c = getc(ifp)) != EOF)
	   putc(c, ofp);
}

/*--------------------------------------------------------------------------- */
/* Check if a file is accessible */
int is_writable(char *file)
{
    struct stat s_buf;

    /* if file can't be accessed (via stat()) return */
    if (stat (file, &s_buf) == -1)
        return -1;
    else if ((s_buf.st_mode & S_IFMT) == S_IFDIR)
        return -2; /* a directory */
    else if (!(s_buf.st_mode & S_IFREG) || access (file, R_OK) == -1)
        /* not a normal file or it is not readable */
        return -3;
    /* legitimate file */
    return 0;
}

