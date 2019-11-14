# script which gives the total number of expected events for MC simulations

# look in backend folder to access dataSets
import sys
sys.path.insert(0, "backend")

from infofile import infos
from dataSets import realList, dataCombos
from getInput import getInput
 
def count(dataKey):
    """
    Function which returns the total number of generated Monte Carlo events
    for a given data set.
    Return 0 for a real data set.
    """
    if dataKey in realList:
        return 0
    else:
        return infos[dataKey]["xsec"] * 10.064 * 1000

# get input from user
# keep asking until answered
chainsValid = False
while (not chainsValid):
    print("Please enter a comma-seperated list of decay chains.")
    print("Write 'text' if you would prefer to read a list from 'input.txt':")
    chains, chainsValid = getInput()
    print()

# iterate over sums of chains from user input
for i in range(len(chains)):

    # loop over chains in the series
    for j in range(len(chains[i])):
        counts = 0 # number of expected events in the chain
        chain = chains[i][j]

        # if the chain corresponds to data in more then one file then calculate counts for each
        # and add them
        if (chain in dataCombos.keys()):
            print(chain+":")
            for subChain in dataCombos[chain]:
                subCount = count(subChain)
                counts += subCount
                print("{:.2f} from {}".format(subCount,subChain))
            print("{:.2f} in total".format(counts))

        # otherwise get the number of expected events for the single file
        else:
            counts += count(chain)
            print("{}: {:.2f}".format(chain,counts))

