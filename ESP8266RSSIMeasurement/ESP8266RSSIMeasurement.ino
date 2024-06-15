#include <ESP8266WiFi.h>
#include <espnow.h>
#include <U8g2lib.h> // from https://github.com/olikraus/u8g2
#include "user_interface.h"

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

int rssi = 0;

uint8_t ignore_address_one[] = {0xCC, 0x7B, 0x5C, 0x28, 0xD4, 0x50};
uint8_t ignore_address_two[] = {0x48, 0xE7, 0x29, 0x6D, 0x38, 0xDC};
uint8_t ESP_OUI[] = {0x18, 0xFE, 0x34};

/**
 * @brief Callback function for handling received packets in promiscuous mode.
 *
 * This function is called when a packet is received in promiscuous mode. It extracts
 * the necessary information from the packet and performs some checks to determine
 * the RSSI (Received Signal Strength Indicator) value.
 *
 * @param buf Pointer to the received packet buffer.
 * @param len Length of the received packet.
 */
void promiscuous_rx_cb(uint8_t *buf, uint16_t len) {
  typedef struct {
    signed rssi:8;
    unsigned rate:5;
    unsigned :3;
    unsigned sig_mode:2;
    unsigned :1;
    unsigned mcs:7;
    unsigned cwb:1;
    unsigned :8;
    unsigned smoothing:1;
    unsigned not_sounding:1;
    unsigned :1;
    unsigned aggregation:1;
    unsigned stbc:2;
    unsigned fec_coding:1;
    unsigned sgi:1;
    unsigned noise_floor:8;
    unsigned ampdu_cnt:8;
    unsigned channel:4;
    unsigned :12;
  } wifi_pkt_rx_ctrl_t;

  typedef struct {
    wifi_pkt_rx_ctrl_t rx_ctrl;
    uint8_t payload[0];
  } wifi_promiscuous_pkt_t;

  typedef struct {
    unsigned frame_ctrl: 16;
    unsigned duration_id: 16;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    unsigned sequence_ctrl: 16;
    unsigned category: 8;
    uint8_t addr4[6];
  } wifi_ieee80211_mac_hdr_t;

  typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0];
  } wifi_ieee80211_packet_t;

  static const uint8_t ACTION_SUBTYPE = 0xd0;
  const wifi_promiscuous_pkt_t *package = (wifi_promiscuous_pkt_t *)buf;
  const wifi_ieee80211_packet_t *pk = (wifi_ieee80211_packet_t*)package->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &pk->hdr;
  const uint8_t *data = pk->payload;

  if ((ACTION_SUBTYPE == (hdr->frame_ctrl & 0xFF)) &&
      (memcmp(hdr->addr2, ignore_address_one, 6) != 0) &&
      (memcmp(hdr->addr2, ignore_address_two, 6) != 0) &&
      (memcmp(hdr->addr4, ESP_OUI, 3) == 0)) {
    rssi = package->rx_ctrl.rssi;
  }
}

/**
 * Initializes the ESP8266 module and sets up necessary configurations.
 */
void setup(void) {
  Serial.begin(115200);

  u8g2.begin();

  WiFi.mode(WIFI_STA);

  wifi_promiscuous_enable(1);
  wifi_set_promiscuous_rx_cb(promiscuous_rx_cb);
}

/**
 * The main loop function that runs repeatedly in the program.
 * It clears the display buffer, sets the font, and displays the RSSI value on the screen.
 * It also prints the RSSI value to the serial monitor.
 * Finally, it sends the buffer to the display and adds a delay of 1 second.
 */
void loop(void) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_u8glib_4_tf);
  u8g2.drawStr(56, 17, "RSSI");

  u8g2.setFont(u8g2_font_8x13B_tr);

  char str_rssi[4];
  sprintf(str_rssi, "%d", rssi);
  u8g2.drawUTF8(50, 32, str_rssi);

  Serial.print("RSSI: ");
  Serial.println(rssi);
  u8g2.sendBuffer();
  delay(1000);
}
