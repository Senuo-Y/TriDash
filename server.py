import socket
import cv2
import struct
import pickle

HOST = "127.0.0.1"
PORT = 65432

def check_camera_available():
    cap = cv2.VideoCapture(0)
    available = cap.isOpened()
    cap.release()
    return available

def stream_frames(conn):
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("[SERVER] Camera failed during stream.")
        return

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break

            _, jpeg = cv2.imencode(".jpg", frame)
            data = jpeg.tobytes()
            # Send size first
            conn.sendall(struct.pack(">I", len(data)))
            # Send frame
            conn.sendall(data)
    except Exception as e:
        print(f"[SERVER] Streaming error: {e}")
    finally:
        cap.release()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print(f"[SERVER] Listening on {HOST}:{PORT}")

    while True:
        conn, addr = s.accept()
        with conn:
            print(f"[SERVER] Connected by {addr}")
            request = conn.recv(1, 0)
            print(request)

            if request == b'I':  # "I" = Inquiry
                print("[SERVER] Client asked for camera availability.")
                available = check_camera_available()
                if available:
                    print("SENDING AVAILABLE!")
                else:
                    print("welp")
                conn.sendall(b'1' if available else b'0')

            elif request == b'S':  # "S" = Stream
                print("[SERVER] Starting frame stream...")
                stream_frames(conn)
