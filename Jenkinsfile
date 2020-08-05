def publish_to_influxdb()  {
        // version >= 2.0
        def influxdb = Jenkins.instance.getDescriptorByType(jenkinsci.plugins.influxdb.InfluxDbStep.DescriptorImpl)

        // Create target
        def target = new jenkinsci.plugins.influxdb.models.Target()

        // Set target details

        // Mandatory fields
        target.description = 'Post-Build Target'
        target.url = 'http://192.168.49.248:8086'
        target.username = 'cip-user'

        // version < 2.0
        target.password = hudson.util.Secret.fromString(''6y9JhiktEMzxd7')

        // version >= 2.0
        //target.password = hudson.util.Secret.fromString('my-password')

        target.database = 'influxdb_plugin'

        // Add a target by using the created target object
        influxdb.addTarget(target)
        influxdb.save()

        // Write stuff to InfluxDB
        influxDbPublisher(selectedTarget: 'Post-Build Target')

        // Remove a target by using the target description field value
        influxdb.removeTarget('Post-Build Target')
        influxdb.save()        
}

pipeline{
    agent any
    stages{
        stage('Init'){
            steps{
                echo "Testing . "
            }
        }
        stage('Build'){
            steps{
                echo 'Building Project . . '
            }
        }
        stage('Deploy'){
            steps{
                echo 'code deployed'
            }
        }
    }
    post
    {
        always {
          publish_to_influxdb()
        }
    }
}
