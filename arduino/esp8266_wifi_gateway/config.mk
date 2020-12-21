THIS_DIR := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))
ROOT := $(THIS_DIR)/..
LIBS =  \
  $(ROOT)/libs/

UPLOAD_SPEED = 115200

BUILD_EXTRA_FLAGS = \
    -DSTASSID=\"$(ssid)\" \
    -DSTAPSK=\"$(apsk)\" \
    -DMQTT_BROKER=\"$(mqtt-broker)\" \
    -DMQTT_PORT=$(mqtt-port) \
    -DMQTT_USERNAME=\"$(mqtt-user)\" \
    -DMQTT_PASSWORD=\"$(mqtt-password)\" 
