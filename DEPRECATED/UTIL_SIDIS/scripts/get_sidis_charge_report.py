#!/usr/bin/env python
import sys
runNo = sys.argv[1]
evenNo = -1
#sys.argv[2]

#Create reduced report output file
out_filename = './UTIL_SIDIS/REPORT/sidis_charge_report_%s.report'% (runNo)

#Read REPORT File Name
report_filename = './REPORT_OUTPUT/COIN/SCALERS/replay_coin_scalers_%s_%s.report'% (runNo, evenNo)

line_no=0

f = open(report_filename)
fout = open(out_filename,'w')

fout.write('************************************************************\n')
fout.write('******************WRITE TO RUN SHEET************************\n')
fout.write('************************************************************\n')

fout.write('\n')

for line in f:
    data = line.split(':')
    if ('Run #' in data[0])  or ('SHMS BCM4a Beam Cut Charge'in data[0]):
    
        line_no = line_no+1
        
        print data[0]
        fout.write(data[0]+ ': '+ data[1])
        
fout.close()
f.close()



#reopen output file to rename some variables
