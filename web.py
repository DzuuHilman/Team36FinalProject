import streamlit as st
import os
import time
from pydub import AudioSegment
from pydub.playback import play

# Fungsi untuk memutar file MP3
def play_mp3(mp3_file):
    audio = AudioSegment.from_file(mp3_file)
    play(audio)

# Fungsi untuk memantau perubahan file
def watch_file(file_path):
    last_modified_time = os.path.getmtime(file_path)
    while True:
        time.sleep(8)  # Tunggu 5 detik sebelum memeriksa lagi
        current_modified_time = os.path.getmtime(file_path)
        if current_modified_time != last_modified_time:
            last_modified_time = current_modified_time
            yield True  # Kembali True jika file diperbarui
        else:
            yield False  # Kembali False jika tidak ada perubahan

# Streamlit interface
st.title("MP3 Player")

# Path ke file MP3
tts_folder = 'tts'
filename = "tts.mp3"
file_path = os.path.join(tts_folder, filename)

# Menampilkan status
status_message = st.empty()

# Path ke file MP3 dan gambar
image_file_path = "frames/frame.jpg"  # Ganti dengan path file gambar Anda

# Menampilkan status
status_message = st.empty()

# Mulai pemantauan file
with st.spinner('Memulai pemantauan file MP3...'):
    watcher = watch_file(file_path)
    for change_detected in watcher:
        if change_detected:
            status_message.text("File MP3 diperbarui, memutar ulang...")
            try:
                # Menampilkan gambar
                if os.path.exists(image_file_path):
                    st.image(image_file_path, caption='Gambar', use_column_width=True)
                else:
                    st.error("Gambar tidak ditemukan!")
                st.audio(file_path, format='audio/mp3', autoplay=True)
                status_message.text("File MP3 selesai diputar.")
            except Exception as e:
                status_message.error(f"Gagal memutar file MP3: {e}")
