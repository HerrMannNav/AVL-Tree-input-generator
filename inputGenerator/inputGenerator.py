import random
import math

def main():
    totalSizeOfTheBox = int(1e8) + 1
    numberOfSamples = int(1e7) + 1
    numberOfIntervals = 10 * pow(2,6)          # Should be a divisor of numberOfSamples
    
    random.seed(36476)
    
    outputFileInsertion = open("../data/inputExperiment-Insertion.txt", 'w')
    outputFileDeletion = open("../data/inputExperiment-Deletion.txt", 'w')
    randomNumbersList = random.sample(range(totalSizeOfTheBox), numberOfSamples)
    
    sizeOfInterval = numberOfSamples / numberOfIntervals
    outputFileInsertion.write(str(numberOfSamples + numberOfIntervals + 1) + "\n")
    outputFileInsertion.write("TI " + str(sizeOfInterval) + "\n")
    
    outputFileDeletion.write(str( 2*(numberOfSamples + numberOfIntervals + 1) ) + "\n")
    print "Number of intervals: " + str(numberOfIntervals)
    for interval in range(0, numberOfIntervals):        
        randomNumbersInterval = randomNumbersList[sizeOfInterval*interval:sizeOfInterval*interval+sizeOfInterval]
        
        randomNumbersIntervalDeletions = randomNumbersInterval[1:(len(randomNumbersInterval)/2)]
        for number in randomNumbersInterval:
            outputFileInsertion.write("IN " + str(number) + "\n")
        for number in randomNumbersIntervalDeletions:
            outputFileDeletion.write("IN " + str(number) + "\n")
        
        outputFileInsertion.write("TI " + str(sizeOfInterval) + "\n")
        outputFileDeletion.write("TI " + str(sizeOfInterval/2) + "\n")
        
        for numberToDelete in random.sample(randomNumbersIntervalDeletions, len(randomNumbersIntervalDeletions)):
            outputFileDeletion.write("RE " + str(numberToDelete) + "\n")
        for number in randomNumbersIntervalDeletions:
            outputFileDeletion.write("IN " + str(number) + "\n")
    
    outputFileInsertion.write("IN " + str(randomNumbersList[100]) + "\n")
    
    outputFileInsertion.close()
    outputFileDeletion.close()
    
if __name__ == "__main__":
    main()