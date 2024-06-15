#include <U8g2lib.h> // from https://github.com/olikraus/u8g2
#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);

const uint8_t ignore_address_one[] = {0x48, 0xE7, 0x29, 0x6D, 0x38, 0xDC}; //on screen
const uint8_t ignore_address_two[] = {0xCC, 0x7B, 0x5C, 0x28, 0xD4, 0x50};
const uint8_t ESP_OUI[] = {0x18, 0xFE, 0x34};

int rssi = 0;

/**
 * Callback function for handling received promiscuous packets.
 * This function is called when a promiscuous packet is received. It filters out non-management frames and processes only action frames containing the Espressif OUI.
 * The RSSI value of the received packet is printed to the Serial monitor.
 * 
 * @param buf Pointer to the received packet buffer.
 * @param type Type of the received packet.
 */
void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {

  // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
  if (type != WIFI_PKT_MGMT)
    return;

  static const uint8_t ACTION_SUBTYPE = 0xd0;
  
  typedef struct {
    unsigned frame_ctrl: 16;  
    unsigned duration_id: 16;
    uint8_t receiver_addr[6];
    uint8_t sender_addr[6]; 
    uint8_t filtering_addr[6];
    unsigned sequence_ctrl: 16;
    unsigned category:8;
    uint8_t addr4[6]; // Contains organizationally Unique Identifier
  } wifi_ieee80211_mac_hdr_t;

  typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0]; // network data ended with 4 bytes csum (CRC32) 
  } wifi_ieee80211_packet_t;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

  // Only continue processing if this is an action frame containing the Espressif OUI.
  if ((ACTION_SUBTYPE == (hdr->frame_ctrl & 0xFF)) &&
    (memcmp(hdr->sender_addr, ignore_address_one, 6) != 0) &&
    (memcmp(hdr->sender_addr, ignore_address_two, 6) != 0) &&
    (memcmp(hdr->addr4, ESP_OUI, 3) == 0)) 
    {
    rssi = ppkt->rx_ctrl.rssi;
    Serial.print("RSSI: ");
    Serial.println(rssi);
    }
}

/**
 * Initializes the ESP32 and sets up necessary configurations. 
 * This function is called once at the beginning of the program.
 * It initializes the Serial Monitor, sets the device as a Wi-Fi Station,
 * and enables promiscuous mode for Wi-Fi packet sniffing.
 */
void setup(void) {
  // Init Serial Monitor
  Serial.begin(115200);

  u8g2.begin();
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_MODE_APSTA);

  esp_wifi_set_promiscuous(1);
  esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);
}

/**
 * The main loop function that runs repeatedly in the program.
 * It clears the internal memory, sets the font, writes the RSSI value to the display,
 * prints the RSSI value to the serial monitor, transfers the internal memory to the display,
 * and adds a delay of 1 second.
 */
void loop(void) {

  u8g2.clearBuffer();					// clear the internal memory

  u8g2.setFont(u8g2_font_u8glib_4_tf );	// choose a suitable font
  u8g2.drawStr(56,17,"RSSI");	// write something to the internal memory

  u8g2.setFont(u8g2_font_8x13B_tr);	// choose a suitable font

  char str_rssi[4];
  sprintf(str_rssi, "%d", rssi);
  u8g2.drawUTF8(50,32,str_rssi);	// write something to the internal memory
  Serial.print("RSSI: ");
  Serial.println(rssi);
  
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);  
}
