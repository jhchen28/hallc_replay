#!/usr/bin/env python
import sys
runNo = sys.argv[1]
evenNo = sys.argv[2]

#Create reduced report output file
out_filename = './UTIL_SIDIS/REPORT/sidis_report_%s.report'% (runNo)

#Read REPORT File Name
report_filename = './REPORT_OUTPUT/COIN/PRODUCTION/replay_coin_production_%s_%s.report'% (runNo, evenNo)

line_no=0

f = open(report_filename)
fout = open(out_filename,'w')

fout.write('************************************************************\n')
fout.write('******************WRITE TO RUN SHEET  modified************************\n')
fout.write('************************************************************\n')

fout.write('\n')

for line in f:
    data = line.split(':')
    if ('Run #' in data[0])  or ('SHMS BCM4B Beam Charge'in data[0]) or ('SHMS BCM4B Beam Cut Charge'in data[0]) or ('SHMS BCM4B Beam Current'in data[0]) or ('SHMS BCM4B Beam Cut Current'in data[0]) or ('Coin ROC2 pTRIG1 Accepted Triggers' in data[0]) or ('Coin ROC2 pTRIG3 Accepted Triggers' in data[0]) or ('Coin ROC2 pTRIG5 Accepted Triggers' in data[0]) or ('Coin ROC2 pTRIG1 passed' in data[0]) or ('Coin ROC2 pTRIG3 passed' in data[0]) or ('Coin ROC2 pTRIG5 passed' in data[0]) or ('SHMS_pTRIG1' in data[0]) or ('HMS_pTRIG1' in data[0]) or ('SHMS_pTRIG3' in data[0] ) or ('HMS_pTRIG3' in data[0]) or ('SHMS_pTRIG5' in data[0] ) or ('HMS_pTRIG5' in data[0]) or ('E SING FID TRACK EFFIC' in data[0]) or ('HADRON SING FID TRACK EFFIC' in data[0]) or ('ROC2 Pre-Scaled Ps6 ROC2 Computer Dead Time' in data[0]) or ('TRIG2 Counts/uC Cut Current' in data[0]) or ('TRIG3 Counts/uC Cut Current' in data[0]) or ('TRIG5 Counts/uC Cut Current' in data[0]) or ('TRIG2 Rate Cut Current' in data[0]) or ('TRIG3 Rate Cut Current' in data[0]) or ('TRIG5 Rate Cut Current' in data[0]):
    
        line_no = line_no+1
        
        if line_no == 12:
            continue
            # commented out to make it simple for shift crew
            # add if you need it
            # fout.write('SHMS ' + data[0] + ': ' + data[1])
        elif line_no == 19:
            fout.write('SHMS ' + data[0] + ': ' + data[1])
        elif line_no == 20:
            fout.write('SHMS ' + data[0] + ': ' + data[1])
        elif line_no == 21:
            fout.write('HMS ' + data[0] + ': ' + data[1])
        elif line_no == 22:
            fout.write('HMS ' + data[0] + ': ' + data[1])
       # elif line_no == 25:
            # thats the dead time calculation
            #continue
            # commented out to make it simple for shift crew
            # add if you need it
            # fout.write('HMS ' + data[0]+ ': '+ data[1])
        else: 
            fout.write(data[0]+ ': '+ data[1])
        
fout.close()
f.close()



#reopen output file to rename some variables
