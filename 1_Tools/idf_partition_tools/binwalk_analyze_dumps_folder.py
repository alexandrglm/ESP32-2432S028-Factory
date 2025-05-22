import os
import subprocess

def analyze_binwalk_folder(path):

    for filename in os.listdir(path):

        if filename.endswith('.bin'):

            filepath = os.path.join(path, filename)

            print(f'\nAnalyzing file ->  {filename}')

            try:

                result = subprocess.run(['binwalk', filepath], capture_output=True, text=True)

                print(result.stdout)
                print('\nFile -> Analysis done.\n')

            except Exception as e:

                print(f'[ERROR] {filename} might not be a partition bin -> {e}')




if __name__ == "__main__":

    folder_path = "../0_NANDS/"
    analyze_binwalk_folder(folder_path)
