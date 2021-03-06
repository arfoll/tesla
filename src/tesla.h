// vim: set expandtab:set tabstop=2
/*
 * tesla
 *
 * Copyright (C) 2012 Philippe Cornet <phil.cornet@gmail.com>
 * Copyright (C) 2013 Maxime Hadjinlian <maxime.hadjinlian@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#pragma once

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <usb.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

// DEV_ID and PID
#define OWL_VENDOR_ID 0x0fde
#define CM160_DEV_ID  0xca05

#define CP210X_IFC_ENABLE       0x00
#define CP210X_SET_BAUDRATE     0x1E
// CP210X_IFC_ENABLE
#define UART_ENABLE             0x0001
#define UART_DISABLE            0x0000

// CM160 protocol
#define FRAME_ID_LIVE 0x51
#define FRAME_ID_DB   0x59 // value used to store in the DB (ch1_kw_avg)

#define log(format, ...) if (verbose == 1) fprintf (stderr, format, ## __VA_ARGS__)

static char ID_MSG[11] = {
        0xA9, 0x49, 0x44, 0x54, 0x43, 0x4D, 0x56, 0x30, 0x30, 0x31, 0x01 };
static char WAIT_MSG[11] = {
        0xA9, 0x49, 0x44, 0x54, 0x57, 0x41, 0x49, 0x54, 0x50, 0x43, 0x52 };

extern int verbose;
static char database_path[PATH_MAX] = "/var/lib/tesla.rrd";
static char graph_path[PATH_MAX];

struct cm160_device {
  struct usb_device *usb_dev;
  usb_dev_handle *hdev;
  int epin;  // IN end point address
  int epout; // OUT end point address
};

struct record_data {
  struct tm date;
  double watts;
  double amps;
  // double cost;
  // double ah; // watt hour and ampere hour are the units used inside the db
  // double wh;
};

void sigint_handler (int sig);
static void dump_data (struct record_data *rec);
static void decode (unsigned char *frame, struct record_data *rec);
static int prepare_device (void);
static int scan_usb (void);
static int process (unsigned char *frame);
static void get_data (void);
static inline void usage ();
int main (int argc, char **argv);
