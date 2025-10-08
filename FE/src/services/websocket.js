import io from "socket.io-client";

const SOCKET_URL = "https://deteksisampah.site";

export const socket = io.connect(SOCKET_URL, {
  transports: ["websocket"],
  reconnection: true,
  reconnectionAttempts: 5,
  reconnectionDelay: 2000,
});
