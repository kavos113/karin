plugins {
    alias(libs.plugins.kotlin)
    alias(libs.plugins.kover)
}

group = "com.github.kavos113"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation(libs.junit)
    testImplementation(libs.mockito)
    testImplementation(libs.kotlin.test)
}

kotlin {
    jvmToolchain(24)
}

tasks.test {
    useJUnitPlatform()
}

tasks.withType<JavaCompile> {
    val jniHeaderDir = file("src/main/cpp/include")

    doFirst {
        jniHeaderDir.mkdirs()
    }

    options.compilerArgs.addAll(
        listOf(
            "-h", jniHeaderDir.absolutePath
        )
    )
}