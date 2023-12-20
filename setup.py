import os
import urllib.request
import zipfile
from pathlib import Path
import subprocess

def download_file(url, filepath, progress_bar=True):
    with urllib.request.urlopen(url) as response:
        with open(filepath, 'wb') as f:
            filesize = int(response.headers.get('content-length'))
            downloaded_size = 0
            block_size = 1024 * 8
            while True:
                buffer = response.read(block_size)
                if not buffer:
                    break
                downloaded_size += len(buffer)
                f.write(buffer)
                if progress_bar:
                    percent = downloaded_size / filesize * 100
                    print(f"Downloading: {percent:.1f}% ({downloaded_size/1024:.1f}KB/{filesize/1024:.1f}KB)\r", end='')

def extract_zip(filepath, destination):
    with zipfile.ZipFile(filepath, 'r') as zip_ref:
        zip_ref.extractall(destination)

def install_premake():
    premake_version = "5.0.0-beta2"
    premake_zip_url = f"https://github.com/premake/premake-core/releases/download/v{premake_version}/premake-{premake_version}-windows.zip"
    premake_win64_dir = "./.target/premake/win64"
    premake_license_dir = "./.target/premake"

    premake_exe_path = Path(f"{premake_win64_dir}/premake5.exe")
    if premake_exe_path.exists():
        print("Premake is already installed.")
        return

    print("Installing premake5...")
    os.makedirs(premake_win64_dir, exist_ok=True)
    download_filepath = os.path.join(premake_win64_dir, "premake.zip")
    download_file(premake_zip_url, download_filepath)

    extract_zip(download_filepath, premake_win64_dir)
    os.remove(download_filepath)

    license_zip_url = f"https://github.com/premake/premake-core/raw/v{premake_version}/LICENSE.txt"
    license_filepath = os.path.join(premake_license_dir, "LICENSE.txt")
    download_file(license_zip_url, license_filepath, progress_bar=False)

    print("Installed premake.")

def update_submodules():
    print("Updating submodules...")
    subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

def setup_astra():
    print("Setting up astra...")
    update_submodules()
    install_premake()
    print("Setup complete.")

setup_astra()