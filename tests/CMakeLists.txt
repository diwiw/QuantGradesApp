# ==============================================
# QuantGradesApp — Tests
# ==============================================

message(STATUS "🧪 Configuring tests...")

add_subdirectory(unit)
# Turned off E2E for now, because it has to be done with google extended test framework.
# Now we would work for windows on separate commands to test server http
# add_subdirectory(e2e)
