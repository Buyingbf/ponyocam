#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/addr.h>

LOG_MODULE_REGISTER(bme280_ble, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define COMPANY_ID_CODE 0x0059 /* Nordic Semiconductor ASA */

typedef struct adv_data {
    // uint8_t flags;
    // uint8_t company_id[2];
    // uint8_t temperature[2];
    // uint8_t humidity[2];
    uint16_t company_id;
    uint16_t test_data;
} adv_data_t;

static adv_data_t adv_data = {
    // .company_id = COMPANY_ID_CODE,
    .test_data = 0x1234
};

static const struct bt_le_adv_param *adv_param =
	BT_LE_ADV_PARAM(BT_LE_ADV_OPT_NONE | BT_LE_ADV_OPT_USE_IDENTITY, /* No options specified */
			800, /* Min Advertising Interval 500ms (800*0.625ms) */
			801, /* Max Advertising Interval 500.625ms (801*0.625ms) */
			NULL); /* Set to NULL for undirected advertising */

// Declare advertising packet
static const struct bt_data ad[] = {
	/* STEP 4.1.2 - Set the advertising flags */
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	/* STEP 4.1.3 - Set the advertising packet data  */
	// BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    BT_DATA(BT_DATA_MANUFACTURER_DATA, (unsigned char *)&adv_data, sizeof(adv_data)),

};

int main(void)
{
        int err;
        bt_addr_le_t addr;

        // Set the device address
        err = bt_addr_le_from_str("FF:67:67:67:67:67", "random", &addr);
        if (err) {
            printk("Invalid BT address (err %d)\n", err);
        }

        err = bt_id_create(&addr, NULL);
        if (err < 0) {
            printk("Creating new ID failed (err %d)\n", err);
        }

        // Enable BLE stack
        err = bt_enable(NULL);
        if (err) {
            LOG_ERR("Bluetooth init failed (err %d)\n", err);
            return -1;
        }
        else {
            LOG_INF("Bluetooth initialized\n");
        }

        // Start advertising
        err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), NULL, 0);
        if (err) {
            LOG_ERR("Advertising failed to start (err %d)\n", err);
            return -1;
        }
        else {
            LOG_INF("Advertising successfully started\n");
        }

        while(1) {
            LOG_INF("Hello World! %s\n", CONFIG_BOARD);
            k_msleep(1000);
            adv_data.test_data++;
            bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL, 0);
        }
        return 0;
}
