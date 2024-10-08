import streamlit as st
import os
import time
import base64
import supervision as sv
from ultralytics import YOLO

# Load model
model = YOLO("yolov8s.pt")
bboxannotator = sv.BoxAnnotator()

# Fungsi untuk cek ada perubahan (modify) file
def watch_file(file_path):
    last_modified_time = os.path.getmtime(file_path)
    while True:
        time.sleep(5)  # Tunggu 5 detik sebelum memeriksa lagi
        current_modified_time = os.path.getmtime(file_path)
        if current_modified_time != last_modified_time:
            last_modified_time = current_modified_time
            yield True  # Kembali True jika file diperbarui
        else:
            yield False  # Kembali False jika tidak ada perubahan

# Fungsi untuk autoplay file audio
def autoplay_audio(file_path: str, audio_placeholder):
    with open(file_path, "rb") as f:
        data = f.read()
        b64 = base64.b64encode(data).decode()
        md = f"""
            <audio controls autoplay>
            <source src="data:audio/mp3;base64,{b64}" type="audio/mp3">
            </audio>
            """
        audio_placeholder.empty()
        audio_placeholder.markdown(
            md,
            unsafe_allow_html=True,
        )

# Streamlit interface
st.title("MP3 Player dan Deteksi Objek")

# Path ke file MP3
tts_folder = 'tts'
filename = "tts.mp3"
file_path = os.path.join(tts_folder, filename)

# Path gambar
frame_folder = 'frames'
frame_filename = "annotated_frame.jpg"
image_file_path = os.path.join(frame_folder, frame_filename)

# Menampilkan status
status_message = st.empty()
image_placeholder = st.empty()
audio_placeholder = st.empty()


# Inject custom CSS
st.markdown(
    """
    <style>
    .main {
        min-height: 100vh;
    }
    </style>
    """,
    unsafe_allow_html=True
)

# Tombol untuk memulai pemantauan file
if st.button("Mulai Pemantauan"):
    with st.spinner('Memulai pemantauan file MP3...'):
        watcher = watch_file(file_path)
        for change_detected in watcher:
            if change_detected:
                status_message.text("File MP3 diperbarui, memutar ulang...")
                try:
                    # Menampilkan gambar dan hasil deteksi
                    if os.path.exists(image_file_path):
                        # process_and_display_image(image_file_path)
                        # display_image(image_file_path)
                        with open(image_file_path, "rb") as f:
                            image = f.read()
                        image_placeholder.image(image, caption='Gambar', use_column_width=True)
                    else:
                        image_placeholder.error("Gambar tidak ditemukan!")
                    
                    # Memutar file MP3
                    autoplay_audio(file_path, audio_placeholder)
                    status_message.text("File MP3 selesai diputar.")
                except Exception as e:
                    status_message.error(f"Gagal memutar file MP3: {e}")
            else:
                status_message.text("Tidak ada perubahan pada file MP3.")
