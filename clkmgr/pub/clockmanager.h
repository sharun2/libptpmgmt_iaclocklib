/* SPDX-License-Identifier: BSD-3-Clause
   SPDX-FileCopyrightText: Copyright © 2024 Intel Corporation. */

/** @file
 * @brief The Clock Manager APIs to set up client-runtime.
 *
 * @author Christopher Hall <christopher.s.hall@@intel.com>
 * @copyright © 2024 Intel Corporation.
 *
 */

#ifndef CLOCKMANAGER_H
#define CLOCKMANAGER_H

#ifdef __cplusplus

#include "pub/clkmgr/subscription.h"
#include "pub/clkmgr/timebase_configs.h"
#include <memory>
#include <vector>

__CLKMGR_NAMESPACE_BEGIN

#include <cstdint>
#include <array>

/**
 * @class ClockSyncBase
 * @brief Base class representing common clock synchronization attributes.
 *
 * This class provides a set of common attributes and methods for clock
 * synchronization, including notification timestamp, clock offset, grandmaster
 * identity, offset range, grandmaster change status, synchronization interval,
 * and composite event status.
 */
class ClockSyncBase
{
  public:
    /**
     * @brief Constructor to initialize base class members.
     *
     * Initializes the clock synchronization attributes to default values.
     */
    ClockSyncBase()
        : clockOffset(0),
          offsetInRange(false),
          sourceClockUUID{},
          sourceChanged(false),
          syncInterval(0),
          notificationTimestamp(0),
          compositeEvent(false)
    {}

    /**
     * @brief Gets the notification timestamp.
     * @return The notification timestamp as a uint64_t.
     */
    uint64_t getNotificationTimestamp() const { return notificationTimestamp; }

    /**
     * @brief Sets the notification timestamp.
     * @param timestamp The new notification timestamp.
     */
    void setNotificationTimestamp(uint64_t timestamp)
    { notificationTimestamp = timestamp; }

    /**
     * @brief Gets the clock offset.
     * @return The clock offset as an int64_t.
     */
    int64_t getClockOffset() const { return clockOffset; }

    /**
     * @brief Sets the clock offset.
     * @param offset The new clock offset.
     */
    void setClockOffset(int64_t offset) { clockOffset = offset; }

    /**
     * @brief Gets the grandmaster identity.
     * @return The grandmaster identity as a std::array of 8 uint8_t elements.
     */
    uint64_t getGmIdentity() const { return sourceClockUUID; }

    /**
     * @brief Sets the grandmaster identity.
     * @param identity The new identity as a std::array of 8 uint8_t elements.
     */
    void setGmIdentity(uint64_t identity)
    { sourceClockUUID = identity; }

    /**
     * @brief Checks if the offset is in range.
     * @return True if the offset is in range, false otherwise.
     */
    bool isOffsetInRange() const { return offsetInRange; }

    /**
     * @brief Sets whether the offset is in range.
     * @param in_range True if the offset is in range, false otherwise.
     */
    void setOffsetInRange(bool in_range) { offsetInRange = in_range; }

    /**
     * @brief Checks if the grandmaster has changed.
     * @return True if the grandmaster has changed, false otherwise.
     */
    bool isGmChanged() const { return sourceChanged; }

    /**
     * @brief Sets whether the grandmaster has changed.
     * @param changed True if the grandmaster has changed, false otherwise.
     */
    void setGmChanged(bool changed) { sourceChanged = changed; }

    /**
     * @brief Gets the synchronization interval.
     * @return The synchronization interval as an int64_t.
     */
    int64_t getSyncInterval() const { return syncInterval; }

    /**
     * @brief Sets the synchronization interval.
     * @param interval The new synchronization interval.
     */
    void setSyncInterval(int64_t interval) { syncInterval = interval; }

    /**
     * @brief Checks if the event is composite.
     * @return True if the event is composite, false otherwise.
     */
    bool isCompositeEvent() const { return compositeEvent; }

    /**
     * @brief Sets whether the event is composite.
     * @param composite True if the event is composite, false otherwise.
     */
    void setCompositeEvent(bool composite) { compositeEvent = composite; }

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~ClockSyncBase() {}

  protected:
    int64_t clockOffset;               /**< Clock offset */
    bool offsetInRange;                /**< Offset in range */
    uint64_t sourceClockUUID; /**< Source clock UUID */
    bool sourceChanged;                /**< Source clock UUID changed */
    uint32_t syncInterval;             /**< Sync interval */
    uint64_t notificationTimestamp;    /**< Notification timestamp */
    bool compositeEvent;               /**< Composite event */
};

/**
 * @class PTPClockSync
 * @brief Derived class for PTP-specific attributes.
 *
 * This class extends the ClockSyncBase class to include attributes specific to
 * Precision Time Protocol (PTP) synchronization, such as whether the clock is
 * synced to the primary clock and whether it is IEEE 802.1AS capable.
 */
class PTPClockSync : public ClockSyncBase
{
  public:
    /**
     * @brief Constructor to initialize both base and derived class members.
     *
     * Initializes the PTP-specific attributes to default values.
     */
    PTPClockSync()
        : syncedToPrimaryClock(false),
          asCapable(false)
    {}

    /**
     * @brief Checks if the clock is synced to the primary clock.
     * @return True if the clock is synced to the primary clock, false otherwise.
     */
    bool isSyncedToPrimaryClock() const { return syncedToPrimaryClock; }

    /**
     * @brief Sets whether the clock is synced to the primary clock.
     * @param synced True if the clock is synced to the primary clock, false
     * otherwise.
     */
    void setSyncedToPrimaryClock(bool synced) { syncedToPrimaryClock = synced; }

    /**
     * @brief Checks if the clock is IEEE 802.1AS capable.
     * @return True if the clock is IEEE 802.1AS capable, false otherwise.
     */
    bool isAsCapable() const { return asCapable; }

    /**
     * @brief Sets whether the clock is IEEE 802.1AS capable.
     * @param capable True if the clock is IEEE 802.1AS capable, false otherwise.
     */
    void setAsCapable(bool capable) { asCapable = capable; }

  private:
    bool syncedToPrimaryClock;
    /**< Indicates if the clock is synced to the primary clock */
    bool asCapable;
    /**< Indicates if the clock is IEEE 802.1AS capable */
};

/**
 * @class ChronyClockSync
 * @brief Derived class for Chrony-specific attributes.
 *
 * This class extends the ClockSyncBase class to represent synchronization
 * attributes specific to Chrony, a versatile implementation of the Network
 * Time Protocol (NTP). Currently, it does not add any additional attributes
 * or methods beyond those inherited from ClockSyncBase.
 */
class ChronyClockSync : public ClockSyncBase
{
  public:
    /**
     * @brief Constructor to initialize both base and derived class members.
     *
     * Initializes the Chrony-specific attributes to default values.
     */
    ChronyClockSync() {}

  private:
    /**
     * Add Chrony-specific attributes and methods here if needed in the future.
     */
};

/**
 * @class ClockSyncStateBase
 * @brief Class holding clock sync state configurations.
 *
 * This class provides a base for managing state counts related to clock
 * synchronization, including counts for offset in range, source clock UUID
 * changes, and composite events.
 */
class ClockSyncStateBase
{
  public:
    /**
     * @brief Constructor to initialize base class members.
     *
     * Initializes the state counts to default values (zero).
     */
    ClockSyncStateBase()
        : offsetInRange(0), sourceChanged(0), compositeEvent(0) {}

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~ClockSyncStateBase() {}

    /**
     * @brief Gets the count of offset in range events.
     * @return The count of offset in range events as a uint32_t.
     */
    uint32_t getOffsetInRangeEventCount() const { return offsetInRange; }

    /**
     * @brief Sets the count of offset in range events.
     * @param count The new count of offset in range events.
     */
    void setOffsetInRangeEventCount(uint32_t count) { offsetInRange = count; }

    /**
     * @brief Gets the count of source clock UUID changed events.
     * @return The count of source clock UUID changed events as a uint32_t.
     */
    uint32_t getGmChangedEventCount() const { return sourceChanged; }

    /**
     * @brief Sets the count of source clock UUID changed events.
     * @param count The new count of source clock UUID changed events.
     */
    void setGmChangedEventCount(uint32_t count) { sourceChanged = count; }

    /**
     * @brief Gets the count of composite events.
     * @return The count of composite events as a uint32_t.
     */
    uint32_t getCompositeEventCount() const { return compositeEvent; }

    /**
     * @brief Sets the count of composite events.
     * @param count The new count of composite events.
     */
    void setCompositeEventCount(uint32_t count) { compositeEvent = count; }

  protected:
    uint32_t offsetInRange;  /**< Offset in range state count */
    uint32_t sourceChanged;  /**< Source clock UUID changed state count */
    uint32_t compositeEvent; /**< Composite event state count */
};

/**
 * @class PTPStateCount
 * @brief Derived class for PTP-specific event counts.
 *
 * This class extends the ClockSyncStateBase class to include event counts
 * specific to Precision Time Protocol (PTP) synchronization, such as counts
 * for events where the clock is synced to the primary clock and events where
 * the clock is IEEE 802.1AS capable.
 */
class PTPStateCount : public ClockSyncStateBase
{
  public:
    /**
     * @brief Constructor to initialize both base and derived class members.
     *
     * Initializes the PTP-specific event counts to default values (zero).
     */
    PTPStateCount()
        : syncedToPrimaryClock(0), asCapable(0) {}

    /**
     * @brief Gets the count of events where the clock is synced to the primary
     * clock.
     * @return The count of synced to primary clock events as a uint32_t.
     */
    uint32_t getSyncedToGmEventCount() const { return syncedToPrimaryClock; }

    /**
     * @brief Sets the count of events where the clock is synced to the primary
     * clock.
     * @param count The new count of synced to primary clock events.
     */
    void setSyncedToGmEventCount(uint32_t count) { syncedToPrimaryClock = count; }

    /**
     * @brief Gets the count of events where the clock is IEEE 802.1AS capable.
     * @return The count of IEEE 802.1AS capable events as a uint32_t.
     */
    uint32_t getAsCapableEventCount() const { return asCapable; }

    /**
     * @brief Sets the count of events where the clock is IEEE 802.1AS capable.
     * @param count The new count of IEEE 802.1AS capable events.
     */
    void setAsCapableEventCount(uint32_t count) { asCapable = count; }

  private:
    uint32_t syncedToPrimaryClock;
    /**< Count of events where the clock is synced to the primary clock */
    uint32_t asCapable;
    /**< Count of events where the clock is IEEE 802.1AS capable */
};

/**
 * @class ChronyStateCount
 * @brief Derived class for Chrony-specific event counts.
 *
 * This class extends the ClockSyncStateBase class to represent event counts
 * specific to Chrony, a versatile implementation of the Network Time Protocol
 * (NTP). Currently, it does not add any additional attributes or methods beyond
 * those inherited from ClockSyncStateBase.
 */
class ChronyStateCount : public ClockSyncStateBase
{
  public:
    /**
     * @brief Constructor to initialize both base and derived class members.
     *
     * Initializes the Chrony-specific event counts to default values.
     */
    ChronyStateCount() {}

  private:
    /**
     * Add Chrony-specific attributes and methods here if needed in the future.
     */
};

/**
 * Class to provide APIs to set up and manage the client-runtime.
 * @note the class is singelton
 */
class ClockManager
{
  private:
    ClockManager() = default;

  public:

    /**
     * Fetch single class object
     * @return reference to single class object
     */
    static ClockManager &FetchSingle();

    /**
     * Initialize the Clock Manager library
     * @return true on success
     */
    static bool init();

    /**
     * Establish connection between Client and Proxy
     * @return true on success, false on failure
     */
    static bool connect();

    /**
     * Remove the connection between Client and Proxy
     * @return true on success, false on failure
     */
    static bool disconnect();

    /**
     * Get the time base configurations
     * @return reference to the TimeBaseConfigurations object
     */
    static const TimeBaseConfigurations &get_timebase_cfgs();

    /**
     * @brief Subscribes to clock synchronization events by name of the time base.
     *
     * This function allows a new subscription to be added for monitoring
     * clock synchronization events associated with a specific time base,
     * identified by its name. It updates the current state based on the
     * subscription details.
     *
     * @param[in] newSub Reference to the new subscription details.
     * @param[in] timeBaseName Name of the time base to be subscribed.
     * @param[out] clockSyncMap Reference to the map containing
     * clock synchronization states.
     * @return True on successful subscription, false on failure.
     */
    static bool subscribe_by_name(const ClkMgrSubscription &newSub,
        const std::string &timeBaseName,
        std::map<std::string, std::shared_ptr<ClockSyncBase>> &clockSyncMap);

    /**
     * @brief Subscribes to clock synchronization events.
     *
     * This function allows a new subscription to be added for monitoring
     * clock synchronization events associated with a specific time base.
     * It updates the current state based on the subscription details.
     *
     * @param[in] newSub Reference to the new subscription details.
     * @param[in] timeBaseIndex Index of the time base to be subscribed.
     * @param[out] clockSyncMap Reference to the map containing
     * clock synchronization states.
     * @return True on successful subscription, false on failure.
     */
    static bool subscribe(const ClkMgrSubscription &newSub, size_t timeBaseIndex,
        std::map<std::string, std::shared_ptr<ClockSyncBase>> &clockSyncMap);

    /**
     * @brief Waits for a specified timeout period for any event changes
     * by name of the time base.
     *
     * This function monitors the specified time base, identified by its name,
     * for event changes within a given timeout period. It updates the current
     * event state and count based on the observed changes.
     *
     * @param[in] timeout Timeout period in seconds.
     * @li Use 0 to check without waiting.
     * @li Use -1 to wait indefinitely until an event change occurs.
     * @param[in] timeBaseName Name of the time base to be monitored.
     * @param[out] clockSyncMap Reference to the map containing clock
     * synchronization states.
     * @param[out] clockSyncStateMap Reference to the map containing clock
     * synchronization state counts.
     * @return Result indicating the status of event changes.
     * @li 1 if an event change occurs within the timeout period.
     * @li 0 if no event changes occur.
     * @li -1 if the connection to the Clock Manager Proxy is lost.
     */
    static int status_wait_by_name(int timeout, const std::string &timeBaseName,
        std::map<std::string, std::shared_ptr<ClockSyncBase>> &clockSyncMap,
        std::map<std::string, std::shared_ptr<ClockSyncStateBase>>
        &clockSyncStateMap);

    /**
     * @brief Waits for a specified timeout period for any event changes.
     *
     * This function monitors the specified time base for event changes
     * within a given timeout period. It updates the current event state
     * and count based on the observed changes.
     *
     * @param[in] timeout Timeout period in seconds.
     * @li Use 0 to check without waiting.
     * @li Use -1 to wait indefinitely until an event change occurs.
     * @param[in] timeBaseIndex Index of the time base to be monitored.
     * @param[out] clockSyncMap Reference to the map containing clock
     * synchronization states.
     * @param[out] clockSyncStateMap Reference to the map containing clock
     * synchronization state counts.
     * @return Result indicating the status of event changes.
     * @li 1 if an event change occurs within the timeout period.
     * @li 0 if no event changes occur.
     * @li -1 if the connection to the Clock Manager Proxy is lost.
     */
    static int status_wait(int timeout, size_t timeBaseIndex,
        std::map<std::string, std::shared_ptr<ClockSyncBase>> &clockSyncMap,
        std::map<std::string, std::shared_ptr<ClockSyncStateBase>>
        &clockSyncStateMap);

    /**
     * Retrieve the time of the CLOCK_REALTIME
     * @param[out] ts timestamp of the CLOCK_REALTIME
     * @return true on success
     */
    static bool gettime(timespec &ts);
};

__CLKMGR_NAMESPACE_END
#else /* __cplusplus */
#include "pub/clkmgr/clockmanager_c.h"
#endif /* __cplusplus */

#endif /* CLOCKMANAGER_H */
