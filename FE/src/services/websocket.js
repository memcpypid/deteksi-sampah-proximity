import io from "socket.io-client";

const DEFAULT_SOCKET_URL = "https://deteksisampah.site";
const SOCKET_URL =
  (import.meta && import.meta.env && import.meta.env.VITE_SOCKET_URL) ||
  DEFAULT_SOCKET_URL;

export const socket = io.connect(SOCKET_URL, {
  transports: ["websocket"],
  reconnection: true,
  reconnectionAttempts: 5,
  reconnectionDelay: 2000,
});
