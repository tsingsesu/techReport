#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
小车 UDP 日志接收器 —— 2026 403 无人车项目

用法:
    python car_log.py              # 默认监听 0.0.0.0:8086
    python car_log.py --port 8086

车端要求(见 zf_device_wifi_spi.h):
    WIFI_SPI_AUTO_CONNECT = 2   (UDP 模式)
    WIFI_SPI_TARGET_IP    = "192.168.43.210"   (本机 Wi-Fi 地址)
    WIFI_SPI_TARGET_PORT  = "8086"
    WIFI_SPI_LOCAL_PORT   = "6666"
车端发送: wifi_spi_send_string("...\\r\\n") 即可, 本程序按 \\n 切行打印。

常用检查:
    IPv4 地址变了: netsh interface ipv4 show addresses  (或 ipconfig)
"""
import argparse
import datetime
import socket
import sys

BUF_SIZE = 4096


def main() -> None:
    ap = argparse.ArgumentParser(description="接收小车 UDP 日志")
    ap.add_argument("--host", default="0.0.0.0", help="监听地址, 默认 0.0.0.0")
    ap.add_argument("--port", type=int, default=8086, help="监听端口, 默认 8086")
    ap.add_argument("--out", default=None,
                    help="同时把日志写入文件(追加), 例如 --out log.txt")
    args = ap.parse_args()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((args.host, args.port))

    # 控制台编码保持系统默认(中文 Windows 为 GBK), 只把不能显示的字符替换掉
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="replace")

    print(f"[ready] UDP 监听 {args.host}:{args.port},  等待小车发数据...", flush=True)
    print("[hint]  小车端 TARGET_IP 必须等于本机 Wi-Fi 的 IPv4 地址", flush=True)
    print("[hint]  Ctrl+C 退出\n", flush=True)

    out_fh = None
    if args.out:
        out_fh = open(args.out, "a", encoding="utf-8")
        print(f"[save] 日志同时写入 {args.out}\n", flush=True)

    pending = b""
    try:
        while True:
            data, addr = sock.recvfrom(BUF_SIZE)
            pending += data
            # 按换行切行; 不完整的尾巴留到下一包
            while b"\n" in pending:
                line, pending = pending.split(b"\n", 1)
                text = line.decode("utf-8", errors="replace").rstrip("\r")
                if not text.strip():
                    continue
                ts = datetime.datetime.now().strftime("%H:%M:%S")
                line_out = f"[{ts}] {addr[0]}:{addr[1]}  {text}"
                print(line_out, flush=True)
                if out_fh:
                    out_fh.write(line_out + "\n")
                    out_fh.flush()
    except KeyboardInterrupt:
        print("\n[exit] 已停止")
    finally:
        if out_fh:
            out_fh.close()
            print(f"[save] 已保存到 {args.out}", flush=True)
        sock.close()


if __name__ == "__main__":
    main()
