100 REM     QVALUES.BAS         SEPTEMBER 28, 1990         J. KANTELE

110 COLOR 14, 5: CLS

120 PRINT "*****************************************************************"

130 PRINT " PROGRAM TO CALCULATE NUCLEAR-REACTION Q-VALUES FROM GIVEN MASSES."

140 PRINT " HINT: TO AVOID CONFUSION, USE ATOMIC MASSES ONLY. "

150 PRINT " THE REACTION IS: M1 + M2 = M3 + M4 + ......"

160 PRINT "*****************************************************************"

170 PRINT

180 PRINT " WOULD YOU LIKE TO SEE A LIST OF THE LIGHTEST ATOMIC MASSES? "

190 PRINT

200 INPUT " 1 = YES; 2 = NO; YOUR CHOICE = ", L

210 PRINT

220 IF L = 1 THEN GOSUB 470

230 INPUT " MASS M1 (IN ATOMIC MASS UNITS; u) = ", M1

240 INPUT " MASS M2 (IN ATOMIC MASS UNITS; u) = ", M2

250 PRINT

260 INPUT " NUMBER OF PARTICLES AFTER REACTION N = ", N

270 PRINT : M = 0

280 INPUT " DO YOU KNOW THE MASSES M3, M4,..? (1 = YES; 2 = NO) ANSW. = ", K

290 IF K = 2 THEN GOSUB 600

300 PRINT

310 FOR I = 1 TO N

320 INPUT " FINAL MASSES M3, M4,... (ONE PER LINE) = ", M(I)

330 M = M + M(I)

340 NEXT I

350 Q = (M1 + M2 - M) * 931.4943

360 PRINT

370 PRINT USING " THE REACTION Q-VALUE IS Q = ##.####^^^^ MeV "; Q

380 PRINT

390 PRINT " HOW WOULD YOU LIKE TO CONTINUE? THE CHOICES ARE:"

400 PRINT

410 PRINT " 1 = CONTINUE WITH A LIST OF LIGHT ATOMS;"

420 INPUT " 2 = CONTINUE WITH NO LIST; 3 = QUIT.  YOUR CHOICE = ", C

430 PRINT : IF C = 1 THEN L = 1

440 PRINT "*****************************************************************"

450 PRINT : ON C GOTO 220, 230, 460

460 END

470 COLOR 11: PRINT " n = 1.008 665 u;   H-1 = 1.007 825 u;  H-2 = 2.014 102 u; "

480 PRINT " H-3 = 3.016 049 u; He-3 = 3.016 029 u; He-4 = 4.002 603 u; "

490 PRINT " 1 u = 931.494 32(28) MeV = 1.660 540 2(10) x 1O^(-27) kg. "

500 PRINT

510 PRINT " NOTE: These are ATOMIC masses. 'Bare' projectiles, p, d, t, tau,"

520 PRINT " alpha, miss electrons. Such 'stripped' masses are often listed, "

530 PRINT " and can be used, but then YOU MUST ADD the electron masses to"

540 PRINT " the projectile/ejectile masses (1 electron mass = 510.999 06 keV "

550 PRINT " = .000 548 58 u). Usually, the binding energies can be neglected."

560 COLOR 14: PRINT

570 PRINT "*****************************************************************"

580 PRINT

590 RETURN

600 PRINT

610 PRINT " TO GET THE MASS OF AN UNSTABLE ISOTOPE, ADD RELEVANT BETA-DECAY "

620 PRINT " ENERGIES, ALONG THE MASS PARABOLA, TO THE MASS OF THE CLOSEST "

630 PRINT " STABLE NUCLEUS. FOR EXAMPLE, TO OBTAIN THE MASS OF Ga-62, SUM UP "

640 PRINT " THE BETA-DECAY ENERGIES OF Ga-62 (E = 9.17 MeV), Zn-62 (E = 1.63 "

650 PRINT " MeV), AND Cu-62 (3.95 MeV), DIVIDE BY 931.494 32, AND ADD TO THE "

660 PRINT " MASS OF Ni-62 (= 61.928346 u). THE INFORMATION NEEDED IS FOUND "

670 PRINT " IN Chart of the Nuclides, IN REFERENCES LIKE [Br86], ETC. "

680 RETURN

