import uproot
import matplotlib.pyplot as plt
import numpy as np
import sys
from brokenaxes import BrokenAxes

# Conversion constants
ADC_TO_VOLT = 2.415 / (2 ** 12)  # Volts per ADC unit
NS_PER_ADC_SAMPLE = 2  # Nanoseconds per sample

thr = 0.04  # Threshold for jitter detection
def plot_waveforms(root_file_path, channels,slotname, histjitter):
    slot_tuples = []
    # Open the ROOT file
    with uproot.open(root_file_path) as file:
        # Number of channels
        n_channels = len(channels)

        print(file)
        
        # Create a canvas with subplots (one per channel)
        fig, axes = plt.subplots(n_channels, 1, figsize=(10, 2 * n_channels), sharex=True)
        if n_channels == 1:
            axes = [axes]  # Ensure axes is iterable for a single channel
        
        # Loop through each specified channel
        for idx, channel_num in enumerate(channels):
            dir_name = f"{channel_num}"  # Construct directory name
            if dir_name not in file:
                print(f"Warning: Directory {dir_name} not found in ROOT file.")
                continue
            
            channel = file[dir_name]
            # Get all histograms in the directory
            hist_keys = [key for key in channel.keys() if channel[key].classname == 'TH1F']
            
            ax = axes[idx]
            ax.set_title(f'{slotname}, channel: {channel_num}, # waveforms: {len(hist_keys)}')
            ax.set_ylabel('Amplitude (V)')
            
            # Loop through each histogram in the current directory
            jitter = []
            for hist_key in hist_keys:
                hist = channel[hist_key]
                # Extract bin contents (y-values) and convert to volts
                baseline = np.mean(hist.values()[0:200] * ADC_TO_VOLT)
                y = hist.values() * ADC_TO_VOLT - baseline
                # print(len(hist.values()* ADC_TO_VOLT))
                # Generate x-values (time in nanoseconds)
                x = np.arange(len(y)) * NS_PER_ADC_SAMPLE
                #jitter.append(int(np.where(y > thr)[0][0] * NS_PER_ADC_SAMPLE))
                crossings = np.where(y > thr)[0]
                if len(crossings) > 0:
                    jitter.append(int(crossings[0] * NS_PER_ADC_SAMPLE))
                else:
                    #print(f"No points above threshold for histogram {hist_key} in channel {channel_num}")
                    jitter.append(None)  # Or append a default value like 0
                #print(f"Jitter : {jitter}")
                # Plot the waveform
                if histjitter == 0:
                    ax.plot(x, y, label=hist_key.split(';')[0], alpha=0.7,linewidth=0.3, color='blue')
            
            # Add legend if there are multiple histograms

            jitter = np.array(jitter)
            if jitter.size == 0 or np.all(jitter == None): 
                print(f"no values in jitter array for channel {channel_num}")
            else:
                sigma = np.std(jitter, ddof=1)
                rms = np.sqrt(np.mean(jitter**2))
                #print(f"Jitter: {jitter}")
                #print("Standard Deviation (sigma):", sigma)
                #print("RMS:", rms)
                myjitter = np.max(jitter) - np.min(jitter)
                print(f"Channel {channel_num}, jitter diff: {myjitter} ns")
                slot_tuples.append((channel_num, int(myjitter)))
                #ax.plt.hist(jitter, bins=100, edgecolor='black', alpha=0.7)
                if histjitter == 1:
                    ax.set_title(f'{slotname}, Channel: {channel_num}, # waveforms: {len(hist_keys)}, Jitter diff: {myjitter} ns')
                    ax.set_ylabel('Counts')
                    ax.set_xlabel('Time (ns)')
                    ax.grid(True, alpha=0.3)
                    ax.tick_params(axis='both', labelsize=12)
                    ax.hist(jitter, bins=5, edgecolor='black', alpha=0.7)
                    #print(jitter) 
    
        # Set common x-label
        plt.xlabel('Time (ns)')
        plt.tight_layout()
        
        # Display the plot
        #plt.show()
        if histjitter == 0:
            png_name = f"waveforms_run_{run_name}_{slotname}.pdf"
            plt.savefig(png_name)
        else:
            png_name = f"hist_run_{run_name}_{slotname}.pdf"
            plt.savefig(png_name)

        return slot_tuples
        #plt.show()

if len(sys.argv) != 2:
    print(f"Usage: python {sys.argv[0]} 'run_number, histogram_jitter'")
    sys.exit(1)


try:
    args = sys.argv[1].split(',')
    if len(args) != 2:
        raise ValueError
    run_name, ishistogram = args
    ishistogram = int(ishistogram)
except ValueError:
    print("Error: Argument must be run name and histogram=1 or waveforms=0 separated by commas (e.g., '5641,1')")
    sys.exit(1)

# Define slots as lists of channel numbers
slot4 = [416, 417, 418, 419]
slot5 = [420, 421, 422, 423]
slot6 = [424, 425, 426, 427]
slot7 = [432, 433, 434, 435]
slot10 = [436, 437, 439]
slot11 = [440, 441, 442, 443]
slot12 = [448, 449, 450, 451]
slot13 = [452, 453, 454, 455]
slot14 = [456, 457, 458, 459]
slot15 = [10001, 10002, 10003, 10004]

slot_list = [
    ("slot4", slot4),
    ("slot5", slot5),
    ("slot6", slot6),
    ("slot7", slot7),
    ("slot10", slot10),
    ("slot11", slot11),
    ("slot12", slot12),
    ("slot13", slot13),
    ("slot14", slot14),
    ("slot15", slot15)
]

# make a list of all slots
all_slots = slot4 + slot5 + slot6 + slot7 + slot10 + slot11 + slot12 + slot13 + slot14 + slot15
#print(all_slots)

# Construct ROOT file path with run name
file = f"data/test_PrintADCDataWaves_R{run_name}S0p0.root"
print(f"Opening ROOT file: {file}")

#plot_waveforms(file, slot4,slot4)
all_slot_tuples = []

for slot_name, slot_channels in slot_list:
    print(f"Processing {slot_name}")
    slot_tuples = plot_waveforms(file, slot_channels, slot_name, ishistogram)
    all_slot_tuples.extend(slot_tuples)


print(all_slot_tuples)
slot_dict = {channel: jitter for channel, jitter in all_slot_tuples}
print(slot_dict)

final_dict = {}

for slot_name, slot_channels in slot_list:
    #print(f"Processing {slot_channels} {slot_name}")
    for key in slot_channels:
        #print(f"Processing channel {key} in {slot_name}")
        jitter = slot_dict.get(key, None)
        if jitter is None:
            print(f"Channel {key} not found")
            final_dict[key] = 0
        else:
            print(f"Jitter value for channel {key}: {jitter}")
            final_dict[key] = jitter

print(final_dict)
# plot a histogram of the jitter values and x axis as channel numbers

# Define colors for each slot
colors = plt.cm.tab10(np.linspace(0, 1, len(slot_list)))

# Create figure with two subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 6), sharey=True, gridspec_kw={'width_ratios': [3, 1], 'wspace': 0.05})

# Plot bars for each slot
for idx, (slot_name, channels) in enumerate(slot_list):
    valid_channels = [ch for ch in channels if ch in final_dict]
    jitter_values = [final_dict[ch] for ch in valid_channels]
    
    # Split channels between axes
    low_channels = [ch for ch in valid_channels if ch < 10000]
    high_channels = [ch for ch in valid_channels if ch >= 10000]
    low_jitter = [final_dict[ch] for ch in low_channels]
    high_jitter = [final_dict[ch] for ch in high_channels]
    
    if low_channels:
        ax1.bar(low_channels, low_jitter, label=slot_name, color=colors[idx], alpha=0.8)
    if high_channels:
        ax2.bar(high_channels, high_jitter, label=slot_name, color=colors[idx], alpha=0.8)

# Customize the plot
ax1.set_xlabel('Channel Number (400-500)')
ax2.set_xlabel('Channel Number (10000-10005)')
ax1.set_ylabel('Jitter Value')
ax1.set_title('Jitter values by channel, grouped by slot')

# Set x-axis ticks
low_channels_all = [ch for slot in slot_list for ch in slot[1] if ch < 10000 and ch in final_dict]
high_channels_all = [ch for slot in slot_list for ch in slot[1] if ch >= 10000 and ch in final_dict]
ax1.set_xticks(low_channels_all)
ax2.set_xticks(high_channels_all)
ax1.set_xticklabels(low_channels_all, rotation=45, ha='right')
ax2.set_xticklabels(high_channels_all, rotation=45, ha='right')

# Add legend (only on ax1 to avoid duplication)
ax1.legend()
ax2.legend()
# Add "..." to indicate break
ax1.text(1.02, 0.5, '...', transform=ax1.transAxes, fontsize=14, verticalalignment='center')
ax2.text(-0.02, 0.5, '...', transform=ax2.transAxes, fontsize=14, verticalalignment='center', horizontalalignment='right')

# Adjust layout
plt.tight_layout()

png_name = f"Summary_run_{run_name}.pdf"
plt.savefig(png_name)

# Show the plot
#plt.show()
