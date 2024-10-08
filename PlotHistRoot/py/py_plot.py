######################################################################
# Using uproot
# M. Ascencio-Sosa, Aug 25, 2024
import uproot
import matplotlib.pyplot as plt
import numpy as np

# global vars
file1_path = 'A.root'
file2_path = 'B.root'

hist1_n = 'noOpalVarND_fhc_nocut'
hist2_n = 'wOpalVarND_fhc_nocut'

file1 = uproot.open(file1_path)
file2 = uproot.open(file2_path)

if file1.keys() == file2.keys():
    print("Files have the same keys. Proceeding...")
    print(file1.keys())
    print("Selecting TH2Ds only...................")
else:
    print("Files do not have the same keys. Ending the run.")
    exit()
     
histogram_names = [name.split(";")[0] for name in file1.keys() if file1.classnames()[name] == "TH1D"]

def plot_me(hist_name):
    
    hist1 = file1[hist_name]
    hist2 = file2[hist_name]

    edges1 = hist1.axis().edges()
    values1 = hist1.values()
    errors1 = hist1.variances() ** 0.5  

    edges2 = hist2.axis().edges()
    values2 = hist2.values()
    errors2 = hist2.variances() ** 0.5  

    common_edges = np.intersect1d(edges1, edges2)
    bin_indices1 = np.digitize(edges1[:-1], common_edges) - 1
    bin_indices2 = np.digitize(edges2[:-1], common_edges) - 1

    common_values1 = np.zeros(len(common_edges) - 1)
    common_values2 = np.zeros(len(common_edges) - 1)
    common_errors1 = np.zeros(len(common_edges) - 1)
    common_errors2 = np.zeros(len(common_edges) - 1)

    np.add.at(common_values1, bin_indices1, values1)
    np.add.at(common_values2, bin_indices2, values2)
    np.add.at(common_errors1, bin_indices1, errors1)
    np.add.at(common_errors2, bin_indices2, errors2)

    ratio = np.divide(common_values1, common_values2, where=common_values2 != 0)
    ratio_errors = ratio * np.sqrt((np.divide(common_errors1, common_values1, where=common_values1 != 0))**2 + (np.divide(common_errors2, common_values2, where=common_values2 != 0))**2)

    fig, axs = plt.subplots(2, 1, figsize=(10, 8), gridspec_kw={'height_ratios': [3, 1]})

    axs[0].hist(edges1[:-1], edges1, weights=values1, histtype='stepfilled', alpha=0.4, label=hist1_n, color='blue')
    axs[0].hist(edges2[:-1], edges2, weights=values2, histtype='stepfilled', alpha=0.4, label=hist2_n, color='red')
    axs[0].set_xlabel(hist_name)
    axs[0].set_ylabel('Entries')
    axs[0].legend()

    axs[1].errorbar(common_edges[:-1], ratio, yerr=ratio_errors, fmt='o', label='Ratio', color='black')
    axs[1].axhline(y=1.0, color='gray', linestyle='--', label='Ratio = 1.0')  
    axs[1].set_xlabel(hist_name)
    axs[1].set_ylabel('Ratio')
    axs[1].set_ylim(0, 2)  
    axs[1].legend()

    plt.tight_layout()
    
    plt.savefig(hist_name+'_plt.pdf')
    return 1

status = [plot_me(name) for name in histogram_names]
print("Status of each hist: ", status)

