#define CHECK_DATE_RANGE(obj, field, min, max)                              \
    if (obj.field < min) throw domain_error(#field " is too small"s);       \
    if (obj.field > max) throw domain_error(#field " is too big"s);

void CheckDateTimeValidity(const DateTime& dt) {
    CHECK_DATE_RANGE(dt, year, 1, 9999);
    CHECK_DATE_RANGE(dt, month, 1, 12);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    CHECK_DATE_RANGE(dt, day, 1, month_lengths[dt.month - 1]);
    CHECK_DATE_RANGE(dt, hour, 0, 23);
    CHECK_DATE_RANGE(dt, minute, 0, 59);
    CHECK_DATE_RANGE(dt, second, 0, 59);
}
