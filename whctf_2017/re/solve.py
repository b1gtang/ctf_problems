import sys 
import deadpool_dfa 
import phoenixAES 
import binascii 

def processinput(iblock, blocksize): 
    p = bytes.fromhex('%0*x' % (2*blocksize, iblock)).decode('ascii') 
    print(p) 
    return (None,[p]) 

def processoutput(output, blocksize): 
    num = int(binascii.hexlify(output[:16]), 16) 
    if num != 138562705040537042133148046729108755018:
        print (num) 
    return num

engine=deadpool_dfa.Acquisition(targetbin='./whitebox', targetdata='./whitebox', goldendata='./whitebox.gold', dfa=phoenixAES, processinput=processinput, processoutput=processoutput, encrypt = True ,verbose = True , faults=[('nop', lambda x: 0x90)],maxleaf=1024, minleaf=1, minleafnail=1) 

tracefiles_sets=engine.run() 
for tracefile in tracefiles_sets[0]: 
    if phoenixAES.crack(tracefile): 
        break
